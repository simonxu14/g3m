package org.glob3.mobile.generated; 
//
//  FloatBufferBuilderFromCartesian3D.hpp
//  G3MiOSSDK
//
//  Created by José Miguel S N on 06/09/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//



public class FloatBufferBuilderFromCartesian3D extends FloatBufferBuilder
{
  private final int _centerStrategy;
  private float _cx;
  private float _cy;
  private float _cz;

  private void setCenter(double x, double y, double z)
  {
    _cx = (float) x;
    _cy = (float) y;
    _cz = (float) z;
  }


  public FloatBufferBuilderFromCartesian3D(int centerStrategy, Vector3D center)
  {
     _centerStrategy = centerStrategy;
    setCenter(center._x, center._y, center._z);
  }

  public final void add(Vector3D vector)
  {
    add(vector._x, vector._y, vector._z);
  }

  public final void add(double x, double y, double z)
  {
    if (_centerStrategy == CenterStrategy.firstVertex())
    {
      if (_values.size() == 0)
      {
        setCenter(x, y, z);
      }
    }

    if (_centerStrategy == CenterStrategy.noCenter())
    {
      _values.add((float) x);
      _values.add((float) y);
      _values.add((float) z);
    }
    else
    {
      _values.add((float)(x - _cx));
      _values.add((float)(y - _cy));
      _values.add((float)(z - _cz));
    }
  }

  public final void add(float x, float y, float z)
  {
    if (_centerStrategy == CenterStrategy.firstVertex())
    {
      if (_values.size() == 0)
      {
        setCenter(x, y, z);
      }
    }

    if (_centerStrategy == CenterStrategy.noCenter())
    {
      _values.add(x);
      _values.add(y);
      _values.add(z);
    }
    else
    {
      _values.add(x - _cx);
      _values.add(y - _cy);
      _values.add(z - _cz);
    }
  }

  public final Vector3D getCenter()
  {
    return new Vector3D(_cx, _cy, _cz);
  }

}