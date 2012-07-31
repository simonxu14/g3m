package org.glob3.mobile.generated; 
//
//  IImage.h
//  G3MiOSSDK
//
//  Created by José Miguel S N on 01/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//



public abstract class IImage
{
  // a virtual destructor is needed for conversion to Java
  public void dispose()
  {
  }

//C++ TO JAVA CONVERTER WARNING: 'const' methods are not available in Java:
//ORIGINAL LINE: virtual int getWidth() const = 0;
  public abstract int getWidth();
//C++ TO JAVA CONVERTER WARNING: 'const' methods are not available in Java:
//ORIGINAL LINE: virtual int getHeight() const = 0;
  public abstract int getHeight();

//C++ TO JAVA CONVERTER WARNING: 'const' methods are not available in Java:
//ORIGINAL LINE: virtual IImage* combineWith(const IImage& transparent, int width, int height) const = 0;
  public abstract IImage combineWith(IImage transparent, int width, int height);

//C++ TO JAVA CONVERTER WARNING: 'const' methods are not available in Java:
//ORIGINAL LINE: virtual IImage* combineWith(const IImage& other, const Rectangle& rect, int width, int height) const = 0;
  public abstract IImage combineWith(IImage other, Rectangle rect, int width, int height);

//C++ TO JAVA CONVERTER WARNING: 'const' methods are not available in Java:
//ORIGINAL LINE: virtual IImage* subImage(const Rectangle& rect) const = 0;
  public abstract IImage subImage(Rectangle rect);

//C++ TO JAVA CONVERTER WARNING: 'const' methods are not available in Java:
//ORIGINAL LINE: virtual ByteBuffer* getEncodedImage() const = 0;
  public abstract ByteBuffer getEncodedImage();

}