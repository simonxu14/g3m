//
//  QuadTree.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 7/10/13.
//
//

#include "QuadTree.hpp"


QuadTree_Node::~QuadTree_Node() {
  const int elementsSize = _elements.size();
  for (int i = 0; i < elementsSize; i++) {
    delete _elements[i];
  }
  if (_children != NULL) {
    for (int i = 0; i < 4; i++) {
      delete _children[i];
    }

    delete [] _children;
  }
}


bool QuadTree_Node::add(const Sector& sector,
                        const void* element,
                        int maxElementsPerNode,
                        int maxDepth) {

  if (_elements.size() < maxElementsPerNode || _depth >= maxDepth) {
    _elements.push_back( new QuadTree_Element(sector, element) );
    return true;
  }

  if (_children == NULL) {
    _children = new QuadTree_Node*[4];

    const Geodetic2D lower = _sector.lower();
    const Geodetic2D upper = _sector.upper();

    const Angle splitLongitude = Angle::midAngle(lower.longitude(), upper.longitude());
    const Angle splitLatitude  = Angle::midAngle(lower.latitude(),  upper.latitude());

    const Sector sector0(lower,
                         Geodetic2D(splitLatitude, splitLongitude));

    const Sector sector1(Geodetic2D(lower.latitude(), splitLongitude),
                         Geodetic2D(splitLatitude, upper.longitude()));

    const Sector sector2(Geodetic2D(splitLatitude, lower.longitude()),
                         Geodetic2D(upper.latitude(), splitLongitude));

    const Sector sector3(Geodetic2D(splitLatitude, splitLongitude),
                         upper);

    _children[0] = new QuadTree_Node(sector0, this);
    _children[1] = new QuadTree_Node(sector1, this);
    _children[2] = new QuadTree_Node(sector2, this);
    _children[3] = new QuadTree_Node(sector3, this);
  }

  int selectedChildrenIndex = -1;
  bool keepHere = false;
  for (int i = 0; i < 4; i++) {
    QuadTree_Node* child = _children[i];
    if (child->_sector.touchesWith(sector)) {
      if (selectedChildrenIndex == -1) {
        selectedChildrenIndex = i;
      }
      else {
        keepHere = true;
        break;
      }
    }
  }

  if (keepHere) {
    _elements.push_back( new QuadTree_Element(sector, element) );
    return true;
  }

  if (selectedChildrenIndex >= 0) {
    return _children[selectedChildrenIndex]->add(sector,
                                                 element,
                                                 maxElementsPerNode,
                                                 maxDepth);
  }

  ILogger::instance()->logError("Logic error in QuadTree");
  return false;
}

bool QuadTree_Node::visitElements(const Sector& sector,
                                  const QuadTreeVisitor& visitor) const {
  if (!_sector.touchesWith(sector)) {
    return false;
  }

  const int elementsSize = _elements.size();
  for (int i = 0; i < elementsSize; i++) {
    QuadTree_Element* element = _elements[i];
    if (element->_sector.touchesWith(sector)) {
      const bool abort = visitor.visitElement(element->_sector, element->_element);
      if (abort) {
        return true;
      }
    }
  }

  if (_children != NULL) {
    for (int i = 0; i < 4; i++) {
      QuadTree_Node* child = _children[i];
      const bool abort = child->visitElements(sector, visitor);
      if (abort) {
        return true;
      }
    }
  }

  return false;
}

QuadTree::~QuadTree() {

}

bool QuadTree::add(const Sector& sector,
                   const void* element) {
  return _root->add(sector, element, _maxElementsPerNode, _maxDepth);
}

void QuadTree::visitElements(const Sector& sector,
                             const QuadTreeVisitor& visitor) const {
  _root->visitElements(sector, visitor);
}