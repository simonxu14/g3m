package org.glob3.mobile.generated; 
public class GEOObjectParserAsyncTask extends GAsyncTask
{
  public final URL _url;

  private IByteBuffer _buffer;
  private GEORenderer _geoRenderer;
  private GEOSymbolizer _symbolizer;

  private final boolean _isBSON;

  private GEOObject _geoObject;

  public GEOObjectParserAsyncTask(URL url, IByteBuffer buffer, GEORenderer geoRenderer, GEOSymbolizer symbolizer, boolean isBSON)
  {
     _url = url;
     _buffer = buffer;
     _geoRenderer = geoRenderer;
     _symbolizer = symbolizer;
     _isBSON = isBSON;
     _geoObject = null;
  }

  public void dispose()
  {
    if (_buffer != null)
       _buffer.dispose();
    if (_geoObject != null)
       _geoObject.dispose();
  }

  public final void runInBackground(G3MContext context)
  {
    _geoObject = GEOJSONParser.parse(_buffer);

    if (_buffer != null)
       _buffer.dispose();
    _buffer = null;
  }

  public final void onPostExecute(G3MContext context)
  {
    if (_geoObject == null)
    {
      ILogger.instance().logError("Error parsing GEOJSON from \"%s\"", _url.getPath());
    }
    else
    {
      _geoRenderer.addGEOObject(_geoObject, _symbolizer);
      _geoObject = null;
    }
  }
}