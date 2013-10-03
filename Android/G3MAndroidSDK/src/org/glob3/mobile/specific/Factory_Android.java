

package org.glob3.mobile.specific;

import org.glob3.mobile.generated.IByteBuffer;
import org.glob3.mobile.generated.ICanvas;
import org.glob3.mobile.generated.IFactory;
import org.glob3.mobile.generated.IFloatBuffer;
import org.glob3.mobile.generated.IImage;
import org.glob3.mobile.generated.IIntBuffer;
import org.glob3.mobile.generated.IShortBuffer;
import org.glob3.mobile.generated.ITimer;
import org.glob3.mobile.generated.IWebSocket;
import org.glob3.mobile.generated.IWebSocketListener;
import org.glob3.mobile.generated.URL;


public final class Factory_Android
         extends
            IFactory {


   public Factory_Android() {
   }


   @Override
   public ITimer createTimer() {
      return new Timer_Android();
   }


   @Override
   public void deleteTimer(final ITimer timer) {
   }


   @Override
   public void deleteImage(final IImage image) {
      if (image != null) {
         image.dispose();
      }
   }


   @Override
   public IFloatBuffer createFloatBuffer(final int size) {
      return new FloatBuffer_Android(size);
   }


   @Override
   public IByteBuffer createByteBuffer(final byte[] data,
                                       final int length) {
      return new ByteBuffer_Android(data);
   }


   @Override
   public IByteBuffer createByteBuffer(final int length) {
      return new ByteBuffer_Android(length);
   }


   @Override
   public IFloatBuffer createFloatBuffer(final float f0,
                                         final float f1,
                                         final float f2,
                                         final float f3,
                                         final float f4,
                                         final float f5,
                                         final float f6,
                                         final float f7,
                                         final float f8,
                                         final float f9,
                                         final float f10,
                                         final float f11,
                                         final float f12,
                                         final float f13,
                                         final float f14,
                                         final float f15) {
      return new FloatBuffer_Android(f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15);
   }


   @Override
   public IIntBuffer createIntBuffer(final int size) {
      return new IntBuffer_Android(size);
   }


   @Override
   public IShortBuffer createShortBuffer(final int size) {
      return new ShortBuffer_Android(size);
   }


   @Override
   public ICanvas createCanvas() {
      return new Canvas_Android();
   }


   @Override
   public IWebSocket createWebSocket(final URL url,
                                     final IWebSocketListener listener,
                                     final boolean autodeleteListener,
                                     final boolean autodeleteWebSocket) {
      return new WebSocket_Android(url, listener, autodeleteListener, autodeleteWebSocket);
   }


   @Override
   public IShortBuffer createShortBuffer(final IShortBuffer buffer) {
      return new ShortBuffer_Android(buffer);
   }


   @Override
   public IFloatBuffer createFloatBuffer(final IFloatBuffer buffer) {
      return new FloatBuffer_Android(buffer);
   }


   //   @Override
   //   public IShortBuffer createShortBuffer(final short[] array) {
   //      return new ShortBuffer_Android(array);
   //   }
   //   @Override
   //   public IFloatBuffer createFloatBuffer(final float[] array) {
   //      return new FloatBuffer_Android(array);
   //   }

}
