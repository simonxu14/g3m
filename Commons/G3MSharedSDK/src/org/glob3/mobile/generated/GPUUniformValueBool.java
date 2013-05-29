package org.glob3.mobile.generated; 
////////////////////////////////////////////////////////////////////////
public class GPUUniformValueBool extends GPUUniformValue
{
  public final boolean _value;

  public GPUUniformValueBool(boolean b)
  {
     _value = b;
     super(GLType.glBool());
  }

  public final void setUniform(GL gl, IGLUniformID id)
  {
    if (_value)
       gl.uniform1i(id, 1);
    else
       gl.uniform1i(id, 0);
  }
  public final boolean isEqualsTo(GPUUniformValue v)
  {
    return _value == ((GPUUniformValueBool)v)._value;
  }
  public final GPUUniformValue deepCopy()
  {
    return new GPUUniformValueBool(_value);
  }

  public final String description()
  {
    IStringBuilder isb = IStringBuilder.newStringBuilder();
    isb.addString("Uniform Value Boolean: ");
    isb.addBool(_value);
    String s = isb.getString();
    if (isb != null)
       isb.dispose();
    return s;
  }
}