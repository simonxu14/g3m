//
//  GenericQuadTree.h
//  G3MiOSSDK
//
//  Created by Jose Miguel SN on 07/08/13.
//
//

#ifndef __G3MiOSSDK__GenericQuadTree__
#define __G3MiOSSDK__GenericQuadTree__

#include <iostream>

#include "Sector.hpp"
#include "Geodetic2D.hpp"

class GenericQuadTree_Node;

class GenericQuadTreeNodeVisitor {
public:
  virtual ~GenericQuadTreeNodeVisitor() {}

  virtual bool visitNode(const GenericQuadTree_Node* node) = 0;
  virtual void endVisit(bool aborted) const = 0;
};

class GenericQuadTreeVisitor {
public:
  virtual ~GenericQuadTreeVisitor() {

  }

  virtual bool visitElement(const Sector& sector,
                            const void*   element) const = 0;
  virtual bool visitElement(const Geodetic2D& geodetic,
                            const void*   element) const = 0;

  virtual void endVisit(bool aborted) const = 0;

};

///////////////////////////////////////////////////////////////////////////////////////
class GenericQuadTree_Element {
public:
  const void*  _element;

  GenericQuadTree_Element(const void*   element) :
  _element(element)
  {
  }

  virtual bool isSectorElement() const = 0;
  virtual Geodetic2D getCenter() const = 0;
  virtual Sector getSector() const = 0;

  virtual ~GenericQuadTree_Element() {
  }
};

class GenericQuadTree_SectorElement: public GenericQuadTree_Element{
public:
  const Sector _sector;

  GenericQuadTree_SectorElement(const Sector& sector,
                                const void*   element) :
  _sector(sector),
  GenericQuadTree_Element(element){ }
  bool isSectorElement() const { return true;}
  Geodetic2D getCenter() const { return _sector.getCenter();}
  Sector getSector() const { return _sector;}

  ~GenericQuadTree_SectorElement() {}
};

class GenericQuadTree_Geodetic2DElement: public GenericQuadTree_Element {
public:
  const Geodetic2D _geodetic;

  GenericQuadTree_Geodetic2DElement(const Geodetic2D& geodetic,
                                    const void*   element) :
  _geodetic(geodetic),
  GenericQuadTree_Element(element){}
  bool isSectorElement() const { return false;}
  Geodetic2D getCenter() const { return _geodetic;}
  Sector getSector() const { return Sector(_geodetic, _geodetic);}

  ~GenericQuadTree_Geodetic2DElement() {}
};
///////////////////////////////////////////////////////////////////////////////////////


class GenericQuadTree_Node {
private:
  const int     _depth;
  Sector*  _sector;

  //  Sector* _elementsSector;

  std::vector<GenericQuadTree_Element*> _elements;

  GenericQuadTree_Node** _children;

  GenericQuadTree_Node(const Sector& sector,
                       GenericQuadTree_Node* parent) :
  _sector(new Sector(sector)),
  _depth( parent->_depth + 1 ),
  _children(NULL)//,
  //  _elementsSector(new Sector(sector))
  {
  }

  void splitNode(int maxElementsPerNode,
                 int maxDepth);

  //  void computeElementsSector();

  GenericQuadTree_Node* getBestNodeForInsertion(GenericQuadTree_Element* element);

  void increaseNodeSector(GenericQuadTree_Element* element);

public:
  GenericQuadTree_Node(const Sector& sector) :
  _sector(new Sector(sector)),
  _depth(1),
  _children(NULL)//,
  //  _elementsSector(new Sector(sector))
  {
  }

  ~GenericQuadTree_Node();

  Sector getSector() const{ return *_sector;}
  //  Sector getElementsSector() const { return *_elementsSector;}

  bool add(GenericQuadTree_Element* element,
           int maxElementsPerNode,
           int maxDepth);

  bool acceptVisitor(const Sector& sector,
                     const GenericQuadTreeVisitor& visitor) const;

  bool acceptVisitor(const Geodetic2D& geo,
                     const GenericQuadTreeVisitor& visitor) const;

  bool acceptNodeVisitor(GenericQuadTreeNodeVisitor& visitor) const;

  double getInsertionCostInSquaredDegrees(const Sector& sector) const;

  int getDepth() const { return _depth;}
  int getNElements() const { return _elements.size();}
};

class GenericQuadTree {
private:
  GenericQuadTree_Node* _root;

  const int _maxElementsPerNode;
  const int _maxDepth;

  bool add(GenericQuadTree_Element* element);

public:

  GenericQuadTree() :
  _root( NULL ),
  _maxElementsPerNode(1),
  _maxDepth(12)
  {
  }

  ~GenericQuadTree();

  bool add(const Sector& sector, const void* element);

  bool add(const Geodetic2D& geodetic, const void* element);

  bool acceptVisitor(const Sector& sector,
                     const GenericQuadTreeVisitor& visitor) const;

  bool acceptVisitor(const Geodetic2D& geo,
                     const GenericQuadTreeVisitor& visitor) const;

  bool acceptNodeVisitor(GenericQuadTreeNodeVisitor& visitor) const;

};

////////////////////////////////////////////////////////////////////////////

class GenericQuadTree_TESTER {

  class GenericQuadTreeVisitorSector_TESTER: public GenericQuadTreeVisitor {
  public:

    Sector _sec;

    GenericQuadTreeVisitorSector_TESTER(const Sector& s):_sec(s){}

    bool visitElement(const Sector& sector,
                      const void*   element) const{

      if (_sec.isEqualsTo(sector)){
        std::string* s = (std::string*)element;
//        printf("ELEMENT -> %s\n", s->c_str());
        return true;
      }
      return false;
    }

    bool visitElement(const Geodetic2D& geodetic,
                      const void*   element) const{
      return false;
    }

    void endVisit(bool aborted) const{
      if (!aborted){
        printf("COULDN'T FIND ELEMENT");
      }

    }

  };

  class GenericQuadTreeVisitorGeodetic_TESTER: public GenericQuadTreeVisitor {
  public:
    Geodetic2D _geo;

    GenericQuadTreeVisitorGeodetic_TESTER(const Geodetic2D g):_geo(g){}

    bool visitElement(const Sector& sector,
                      const void*   element) const{return false;}

    bool visitElement(const Geodetic2D& geodetic,
                      const void*   element) const{

      if (geodetic.isEqualsTo(_geo)){
        std::string* s = (std::string*)element;
//        printf("ELEMENT -> %s\n", s->c_str());
        return true;
      }
      return false;
    }

    void endVisit(bool aborted) const{
      if (!aborted){
        printf("COULDN'T FIND ELEMENT");
      }
      
    }
    
  };

  class NodeVisitor_TESTER: public GenericQuadTreeNodeVisitor{
  public:
    int _maxDepth;
    int _meanDepth;

    int _maxNEle;
    int _meanElemDepth;
    int _nNodes;
    int _nElem;
    NodeVisitor_TESTER(): _maxDepth(0), _meanDepth(0), _maxNEle(0), _nNodes(0), _meanElemDepth(0), _nElem(0){}
    

    bool visitNode(const GenericQuadTree_Node* node){
      printf("NODE D: %d, NE: %d\n", node->getDepth(), node->getNElements());

      if (node->getNElements() > _maxNEle){
        _maxNEle = node->getNElements();
      }

      if (_maxDepth < node->getDepth()){
        _maxDepth = node->getDepth();
      }

      _meanDepth += node->getDepth();

      _nNodes++;

      _nElem += node->getNElements();

      _meanElemDepth += node->getNElements() * node->getDepth();


      return false;
    }
    void endVisit(bool aborted) const{
      printf("TREE WITH %d ELEM. MAXDEPTH: %d, MEAN NODE DEPTH: %f, MAX NELEM: %d\n, MEAN ELEM DEPTH: %f",
             _nElem,
             _maxDepth,
             _meanDepth / (float)_nNodes,
             _maxNEle,
             _meanElemDepth / (float) _nElem
             );
    }
  };
  
public:
  static void run(int nElements);
  
};
#endif /* defined(__G3MiOSSDK__GenericGenericQuadTree__) */