package org.glob3.mobile.generated; 
//
//  CameraSimpleDragHandler.cpp
//  G3MiOSSDK
//
//  Created by Agustín Trujillo Pino on 28/07/12.
//  Copyright (c) 2012 Universidad de Las Palmas. All rights reserved.
//


//
//  CameraSimpleDragHandlerr.h
//  G3MiOSSDK
//
//  Created by Agustín Trujillo Pino on 28/07/12.
//  Copyright (c) 2012 Universidad de Las Palmas. All rights reserved.
//





public class CameraSimpleDragHandler extends CameraHandler
{

  public final boolean onTouchEvent(TouchEvent touchEvent)
  {
	// only one finger needed
	if (touchEvent.getTouchCount()!=1)
		return false;
  
	switch (touchEvent.getType())
	{
	  case Down:
		onDown(touchEvent);
		break;
	  case Move:
		onMove(touchEvent);
		break;
	  case Up:
		onUp(touchEvent);
	  default:
		break;
	}
  
	return true;
  }
  public final int render(RenderContext rc)
  {
	return MAX_TIME_TO_RENDER;
  }
  public final void initialize(InitializationContext ic)
  {
  }
  public final void onResizeViewportEvent(int width, int height)
  {
  }



  private void onDown(TouchEvent touchEvent)
  {
	_camera0 = new Camera(_camera);
	_currentGesture = Gesture.Drag;
  
	// dragging
	MutableVector2D pixel = touchEvent.getTouch(0).getPos().asMutableVector2D();
	final Vector3D ray = _camera0.pixel2Ray(pixel.asVector2D());
	_initialPoint = _planet.closestIntersection(_camera0.getPosition(), ray).asMutableVector3D();
  
	//printf ("down 1 finger\n");
  }
  private void onMove(TouchEvent touchEvent)
  {
	//_currentGesture = getGesture(touchEvent);
	if (_currentGesture!=Gesture.Drag)
		return;
	if (_initialPoint.isNan())
		return;
  
	final Vector2D pixel = touchEvent.getTouch(0).getPos();
	final Vector3D ray = _camera0.pixel2Ray(pixel);
	final Vector3D pos = _camera0.getPosition();
  
	MutableVector3D finalPoint = _planet.closestIntersection(pos, ray).asMutableVector3D();
	if (finalPoint.isNan())
	{
	  //INVALID FINAL POINT
	  finalPoint = _planet.closestPointToSphere(pos, ray).asMutableVector3D();
	}
  
	_camera.copyFrom(_camera0);
	_camera.dragCamera(_initialPoint.asVector3D(), finalPoint.asVector3D());
  
	//printf ("Moving 1 finger.  gesture=%d\n", _currentGesture);
  }
  private void onUp(TouchEvent touchEvent)
  {
	_currentGesture = Gesture.None;
	_initialPixel = Vector3D.nan().asMutableVector3D();
  
	//printf ("end 1 finger\n");
  }

}