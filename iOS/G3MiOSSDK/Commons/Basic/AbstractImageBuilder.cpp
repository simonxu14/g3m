//
//  AbstractImageBuilder.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 1/3/14.
//
//

#include "AbstractImageBuilder.hpp"

#include "ChangedListener.hpp"
#include "ILogger.hpp"

void AbstractImageBuilder::changed() {
  if (_listener != NULL) {
    _listener->changed();
  }
}

void AbstractImageBuilder::setChangeListener(ChangedListener* listener) {
  if (_listener != NULL) {
    ILogger::instance()->logError("listener already set!");
  }
  _listener = listener;
}
