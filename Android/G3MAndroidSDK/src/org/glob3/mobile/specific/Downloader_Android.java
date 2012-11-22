

package org.glob3.mobile.specific;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.glob3.mobile.generated.G3MContext;
import org.glob3.mobile.generated.IBufferDownloadListener;
import org.glob3.mobile.generated.IDownloader;
import org.glob3.mobile.generated.IImageDownloadListener;
import org.glob3.mobile.generated.URL;

import android.util.Log;


public final class Downloader_Android
         extends
            IDownloader {

   final static String                                      TAG             = "Downloader_Android";

   private final int                                        _maxConcurrentOperationCount;
   private int                                              _requestIdCounter;
   private long                                             _requestsCounter;
   private long                                             _cancelsCounter;
   private final ArrayList<Downloader_Android_WorkerThread> _workers;
   private final Map<String, Downloader_Android_Handler>    _downloadingHandlers;
   private final Map<String, Downloader_Android_Handler>    _queuedHandlers;
   private final int                                        _connectTimeout;
   private final int                                        _readTimeout;

   private boolean                                          _started;
   private final Object                                     _startStopMutex = new Object();

   private G3MContext                                       _context;


   public Downloader_Android(final int maxConcurrentOperationCount,
                             final int connectTimeoutMillis,
                             final int readTimeoutMillis) {
      _started = false;
      _maxConcurrentOperationCount = maxConcurrentOperationCount;
      _requestIdCounter = 1;
      _requestsCounter = 0;
      _cancelsCounter = 0;
      // TODO String or Url as key??
      _downloadingHandlers = new HashMap<String, Downloader_Android_Handler>();
      _queuedHandlers = new HashMap<String, Downloader_Android_Handler>();
      _workers = new ArrayList<Downloader_Android_WorkerThread>(maxConcurrentOperationCount);

      _connectTimeout = connectTimeoutMillis;
      _readTimeout = readTimeoutMillis;
   }


   @Override
   public void start() {
      synchronized (_startStopMutex) {
         if (!_started) {
            for (int i = 0; i < _maxConcurrentOperationCount; i++) {
               final Downloader_Android_WorkerThread da = new Downloader_Android_WorkerThread(this, i);
               _workers.add(da);
            }


            for (final Downloader_Android_WorkerThread worker : _workers) {
               worker.initialize(_context);
            }

            for (final Downloader_Android_WorkerThread worker : _workers) {
               worker.start();
            }

            _started = true;
            Log.i(TAG, "Downloader started");
         }
      }
   }


   @Override
   public void stop() {
      synchronized (_startStopMutex) {
         if (_started) {
            for (final Downloader_Android_WorkerThread worker : _workers) {
               worker.stopWorkerThread();
            }
            _started = false;

            boolean allWorkersStopped;
            do {
               allWorkersStopped = true;
               for (final Downloader_Android_WorkerThread worker : _workers) {
                  if (!worker.isStopped()) {
                     allWorkersStopped = false;
                     try {
                        Thread.sleep(2);
                     }
                     catch (final InterruptedException e) {
                     }
                     break;
                  }
               }
            }
            while (!allWorkersStopped);

            _workers.clear();
            Log.i(TAG, "Downloader stopped");

            //         boolean allStopped = true;
            //         while (_started) {
            //            for (final Downloader_Android_WorkerThread worker : _workers) {
            //               allStopped = allStopped && worker.isStopping();
            //            }
            //         _started = allStopped;
            //         }
         }
      }
   }


   @Override
   public long requestBuffer(final URL url,
                             final long priority,
                             final IBufferDownloadListener listener,
                             final boolean deleteListener) {

      Downloader_Android_Handler handler = null;
      long requestId;

      synchronized (this) {
         _requestsCounter++;
         requestId = _requestIdCounter++;
         handler = _downloadingHandlers.get(url.getPath());

         if (handler == null) {
            handler = _queuedHandlers.get(url.getPath());
            if (handler == null) {
               // new handler, queue it
               handler = new Downloader_Android_Handler(url, listener, priority, requestId);
               _queuedHandlers.put(url.getPath(), handler);
            }
            else {
               // the URL is queued for future download, just add the new listener
               handler.addListener(listener, priority, requestId);
            }
         }
         else {
            // the URL is being downloaded, just add the new listener
            handler.addListener(listener, priority, requestId);
         }
      }

      return requestId;
   }


   @Override
   public long requestImage(final URL url,
                            final long priority,
                            final IImageDownloadListener listener,
                            final boolean deleteListener) {

      Downloader_Android_Handler handler = null;
      long requestId;

      synchronized (this) {
         _requestsCounter++;
         requestId = _requestIdCounter++;
         handler = _downloadingHandlers.get(url.getPath());

         if (handler == null) {
            handler = _queuedHandlers.get(url.getPath());
            if (handler == null) {
               // new handler, queue it
               handler = new Downloader_Android_Handler(url, listener, priority, requestId);
               _queuedHandlers.put(url.getPath(), handler);
            }
            else {
               // the URL is queued for future download, just add the new listener
               handler.addListener(listener, priority, requestId);
            }
         }
         else {
            // the URL is being downloaded, just add the new listener
            handler.addListener(listener, priority, requestId);
         }
      }

      return requestId;
   }


   @Override
   public void cancelRequest(final long requestId) {
      if (requestId < 0) {
         return;
      }

      synchronized (this) {
         _cancelsCounter++;

         boolean found = false;
         Iterator<Map.Entry<String, Downloader_Android_Handler>> iter = _queuedHandlers.entrySet().iterator();

         while (iter.hasNext() && !found) {
            final Map.Entry<String, Downloader_Android_Handler> e = iter.next();
            final String url = e.getKey();
            final Downloader_Android_Handler handler = e.getValue();

            if (handler.removeListenerForRequestId(requestId)) {

               if (!handler.hasListener()) {
                  _queuedHandlers.remove(url);
               }
               found = true;
            }
         }

         if (!found) {
            iter = _downloadingHandlers.entrySet().iterator();

            while (iter.hasNext() && !found) {
               final Map.Entry<String, Downloader_Android_Handler> e = iter.next();
               final Downloader_Android_Handler handler = e.getValue();

               if (handler.cancelListenerForRequestId(requestId)) {
                  found = true;
               }
            }
         }
      }
   }


   public synchronized void removeDownloadingHandlerForUrl(final String url) {
      _downloadingHandlers.remove(url);
   }


   @Override
   public synchronized void initialize(final G3MContext context) {
      _context = context;
      for (final Downloader_Android_WorkerThread worker : _workers) {
         worker.initialize(_context);
      }
   }


   Downloader_Android_Handler getHandlerToRun() {
      long selectedPriority = Long.MIN_VALUE;
      Downloader_Android_Handler selectedHandler = null;
      String selectedURL = null;

      synchronized (this) {

         if (_context == null) {
            return null;
         }

         //         final Iterator<Map.Entry<String, Downloader_Android_Handler>> it = _queuedHandlers.entrySet().iterator();
         //
         //         while (it.hasNext()) {
         for (final Map.Entry<String, Downloader_Android_Handler> e : _queuedHandlers.entrySet()) {
            //            final Map.Entry<String, Downloader_Android_Handler> e = it.next();
            final String url = e.getKey();
            final Downloader_Android_Handler handler = e.getValue();
            final long priority = handler.getPriority();

            if (priority > selectedPriority) {
               selectedPriority = priority;
               selectedHandler = handler;
               selectedURL = url;
            }
         }

         if (selectedHandler != null) {
            // move the selected handler to _downloadingHandlers collection
            _queuedHandlers.remove(selectedURL);
            _downloadingHandlers.put(selectedURL, selectedHandler);
         }
      }

      return selectedHandler;
   }


   @Override
   public String statistics() {
      final StringBuilder_Android sb = new StringBuilder_Android();

      sb.addString("Downloader_Android(downloading=");
      sb.addInt(_downloadingHandlers.size());
      sb.addString(", queued=");
      sb.addInt(_queuedHandlers.size());
      sb.addString(", totalRequests=");
      sb.addLong(_requestsCounter);
      sb.addString(", totalCancels=");
      sb.addLong(_cancelsCounter);

      return sb.getString();
   }


   public int getConnectTimeout() {
      return _connectTimeout;
   }


   public int getReadTimeout() {
      return _readTimeout;
   }


   @Override
   public void onResume(final G3MContext context) {
      start();
   }


   @Override
   public void onPause(final G3MContext context) {
      stop();
   }


   @Override
   public void onDestroy(final G3MContext context) {
      //      final int __DIEGO_AT_WORK;
      stop();
   }


}
