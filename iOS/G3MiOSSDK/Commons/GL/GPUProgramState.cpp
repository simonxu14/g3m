//
//  GPUProgramState.cpp
//  G3MiOSSDK
//
//  Created by Jose Miguel SN on 05/04/13.
//
//

#include "GPUProgramState.hpp"

GPUProgramState::~GPUProgramState(){
  clear();
  delete _uniformNames;
}

void GPUProgramState::clear(){
  _lastProgramUsed = NULL;
#ifdef C_CODE
  for(std::map<std::string, GPUUniformValue*> ::const_iterator it = _uniformValues.begin();
      it != _uniformValues.end();
      it++){
    delete it->second;
  }
#endif
  _uniformValues.clear();
  
#ifdef C_CODE
  for(std::map<std::string, GPUAttributeValue*> ::const_iterator it = _attributesValues.begin();
      it != _attributesValues.end();
      it++){
    delete it->second;
  }
#endif
  //  _attributesEnabled.clear();
  _attributesValues.clear();
}

void GPUProgramState::applyValuesToLinkedProgram() const{
#ifdef C_CODE
  for(std::map<std::string, GPUUniformValue*> ::const_iterator it = _uniformValues.begin();
      it != _uniformValues.end();
      it++){
    std::string name = it->first;
    GPUUniformValue* v = it->second;
    v->setValueToLinkedUniform();
  }
  
  //  for(std::map<std::string, attributeEnabledStruct> ::const_iterator it = _attributesEnabled.begin();
  //      it != _attributesEnabled.end();
  //      it++){
  //    GPUAttribute* a = it->second.attribute;
  //    if (a == NULL){
  //      ILogger::instance()->logError("NO ATTRIBUTE LINKED");
  //    } else{
  //      a->setEnable(it->second.value);
  //    }
  //  }
  
  for(std::map<std::string, GPUAttributeValue*> ::const_iterator it = _attributesValues.begin();
      it != _attributesValues.end();
      it++){
    GPUAttributeValue* v = it->second;
    v->setValueToLinkedAttribute();
  }
#endif
#ifdef JAVA_CODE
  final Object[] uni = _uniformValues.values().toArray();
  for (int i = 0; i < uni.length; i++) {
    ((GPUUniformValue)uni[i]).setValueToLinkedUniform();
  }
  
  //  final Object[] attEnabled = _attributesEnabled.values().toArray();
  //  for (int i = 0; i < attEnabled.length; i++) {
  //    attributeEnabledStruct a = (attributeEnabledStruct) attEnabled[i];
  //    if (a.attribute == null) {
  //      ILogger.instance().logError("NO ATTRIBUTE LINKED");
  //    }
  //    else {
  //      a.attribute.setEnable(a.value);
  //    }
  //  }
  
  final Object[] att = _attributesValues.values().toArray();
  for (int i = 0; i < att.length; i++) {
    ((GPUAttributeValue)att[i]).setValueToLinkedAttribute();
  }
#endif
}


void GPUProgramState::linkToProgram(GPUProgram& prog) const{
  
  if (_lastProgramUsed == &prog){
    return; //Already linked
  }
  
#ifdef JAVA_CODE
  
  final Object[] uni = _uniformValues.values().toArray();
  final Object[] uniNames = _uniformValues.keySet().toArray();
  for (int i = 0; i < uni.length; i++) {
    final String name = (String)uniNames[i];
    final GPUUniformValue v = (GPUUniformValue) uni[i];
    
    final int type = v.getType();
    final GPUUniform u = prog.getUniformOfType(name, type); //Getting uniform from program
    
    if (u == null) {
      ILogger.instance().logError("UNIFORM " + name + " NOT FOUND");
      return;
    }
    v.linkToGPUUniform(u);
  }
  
  final Object[] att = _attributesValues.values().toArray();
  final Object[] attNames = _attributesValues.keySet().toArray();
  for (int i = 0; i < att.length; i++) {
    final String name = (String)attNames[i];
    final GPUAttributeValue v = (GPUAttributeValue)att[i];
    
    GPUAttribute a = null; //Getting attribute from program
    if (!v.getEnabled()){
      a = prog.getGPUAttribute(name);
    } else{
      final int type = v.getType();
      final int size = v.getAttributeSize();
      if (type==GLType.glFloat()){
        a = prog.getGPUAttributeVecXFloat(name,size);
      }
    }
    
    if (a == null){
      ILogger.instance().logError("ATTRIBUTE NOT FOUND " + name);
      return;
    }
    v.linkToGPUAttribute(a);
  }

#endif
#ifdef C_CODE

  for(std::map<std::string, GPUUniformValue*> ::const_iterator it = _uniformValues.begin();
      it != _uniformValues.end();
      it++){
    
    std::string name = it->first;
    GPUUniformValue* v = it->second;
    
    const int type = v->getType();
    
    GPUUniform* u = prog.getUniformOfType(name, type);
    if (u == NULL){
      ILogger::instance()->logError("UNIFORM " + name + " NOT FOUND");
      return;
    }
    v->linkToGPUUniform(u);
  }
  
  for(std::map<std::string, GPUAttributeValue*> ::const_iterator it = _attributesValues.begin();
      it != _attributesValues.end();
      it++){
    
    std::string name = it->first;
    GPUAttributeValue* v = it->second;
    
    GPUAttribute* a = NULL;
    if (!v->getEnabled()){
      a = prog.getGPUAttribute(name);
    } else{
      const int type = v->getType();
      const int size = v->getAttributeSize();
      if (type==GLType::glFloat()){
        a = prog.getGPUAttributeVecXFloat(name,size);
      }
    }
    
    if (a == NULL){
      ILogger::instance()->logError("ATTRIBUTE NOT FOUND " + name);
      return;
    }
    v->linkToGPUAttribute(a);
  }
  
#endif
  
  
  _lastProgramUsed = &prog;
}

void GPUProgramState::applyChanges(GL* gl) const{
  if (_lastProgramUsed == NULL){
    ILogger::instance()->logError("Trying to use unlinked GPUProgramState.");
  }
  applyValuesToLinkedProgram();
  _lastProgramUsed->applyChanges(gl);
}

bool GPUProgramState::setGPUUniformValue(const std::string& name, GPUUniformValue* v){
  
  GPUUniform* prevLinkedUniform = NULL;
  bool uniformExisted = false;
  
  
#ifdef C_CODE
  std::map<std::string, GPUUniformValue*> ::iterator it = _uniformValues.find(name);
  if (it != _uniformValues.end()){
    prevLinkedUniform = it->second->getLinkedUniform();
    delete it->second;
    uniformExisted = true;
  }
#endif
#ifdef JAVA_CODE
  GPUUniformValue pv = _uniformValues.get(name);
  if (pv != null){
    uniformExisted = true;
    prevLinkedUniform = pv.getLinkedUniform();
  }
#endif
  
  v->linkToGPUUniform(prevLinkedUniform);
  _uniformValues[name] = v;
  
  if (!uniformExisted){
    onStructureChanged();
  }
  
  return uniformExisted;
}

bool GPUProgramState::setGPUAttributeValue(const std::string& name, GPUAttributeValue* v){
  GPUAttribute* prevLinkedAttribute = NULL;
  bool attributeExisted = false;
#ifdef C_CODE
  std::map<std::string, GPUAttributeValue*> ::iterator it = _attributesValues.find(name);
  if (it != _attributesValues.end()){
    prevLinkedAttribute = it->second->getLinkedAttribute();
    delete it->second;
    attributeExisted = true;
  }
#endif
#ifdef JAVA_CODE
  GPUAttributeValue pv = _attributesValues.get(name);
  if (pv != null){
    attributeExisted = true;
    prevLinkedAttribute = pv.getLinkedAttribute();
  }
#endif
  
  v->linkToGPUAttribute(prevLinkedAttribute);
  _attributesValues[name] = v;
  
  if (!attributeExisted){
    onStructureChanged();
  }
  
  return attributeExisted;
}

bool GPUProgramState::setAttributeValue(const std::string& name,
                                        IFloatBuffer* buffer, int attributeSize,
                                        int arrayElementSize, int index, bool normalized, int stride){
  switch (attributeSize) {
    case 1:
      return setGPUAttributeValue(name, new GPUAttributeValueVec1Float(buffer, arrayElementSize, index, stride, normalized) );
    case 2:
      return setGPUAttributeValue(name, new GPUAttributeValueVec2Float(buffer, arrayElementSize, index, stride, normalized) );
    case 3:
      return setGPUAttributeValue(name, new GPUAttributeValueVec3Float(buffer, arrayElementSize, index, stride, normalized) );
    case 4:
      return setGPUAttributeValue(name, new GPUAttributeValueVec4Float(buffer, arrayElementSize, index, stride, normalized) );
    default:
      ILogger::instance()->logError("Invalid size for Attribute.");
      return false;
  }
}

bool GPUProgramState::setUniformValue(const std::string& name, bool b){
  return setGPUUniformValue(name, new GPUUniformValueBool(b) );
}

bool GPUProgramState::setUniformValue(const std::string& name, float f){
  return setGPUUniformValue(name, new GPUUniformValueFloat(f));
}

bool GPUProgramState::setUniformValue(const std::string& name, const Vector2D& v){
  return setGPUUniformValue(name, new GPUUniformValueVec2Float(v._x, v._y));
}

bool GPUProgramState::setUniformValue(const std::string& name, double x, double y, double z, double w){
  return setGPUUniformValue(name, new GPUUniformValueVec4Float(x,y,z,w));
}

//bool GPUProgramState::setUniformValue(const std::string& name, const MutableMatrix44D* m){
//
//#ifdef C_CODE
//  for(std::map<std::string, GPUUniformValue*> ::iterator it = _uniformValues.begin();
//      it != _uniformValues.end();
//      it++){
//    std::string thisName = it->first;
//    GPUUniformValue* uv = (GPUUniformValue*)it->second;
//    if (thisName.compare(name) == 0 && uv->getType() == GLType::glMatrix4Float()){
//      GPUUniformValueMatrix4FloatStack* v = (GPUUniformValueMatrix4FloatStack*)it->second;
//      v->loadMatrix(m);
//      return true;
//    }
//  }
//#endif
//#ifdef JAVA_CODE
//  final Object[] uni = _uniformValues.values().toArray();
//  final Object[] uniNames = _uniformValues.keySet().toArray();
//  for (int i = 0; i < uni.length; i++) {
//    final String thisName =  (String)uniNames[i];
//    final GPUUniformValue uv = (GPUUniformValue) uni[i];
//    if ((thisName.compareTo(name) == 0) && (uv.getType() == GLType.glMatrix4Float()))
//    {
//      final GPUUniformValueMatrix4FloatStack v = (GPUUniformValueMatrix4FloatStack)uv;
//      v.loadMatrix(m);
//      return true;
//    }
//  }
//#endif
//
//  return setGPUUniformValue(name, new GPUUniformValueMatrix4FloatStack(m));
//}

//bool GPUProgramState::multiplyUniformValue(const std::string& name, const MutableMatrix44D* m){
//
//#ifdef C_CODE
//
//  for(std::map<std::string, GPUUniformValue*> ::iterator it = _uniformValues.begin();
//      it != _uniformValues.end();
//      it++){
//    std::string thisName = it->first;
//    GPUUniformValue* uv = (GPUUniformValue*)it->second;
//    if (thisName.compare(name) == 0 && uv->getType() == GLType::glMatrix4Float()){
//      GPUUniformValueMatrix4FloatStack* v = (GPUUniformValueMatrix4FloatStack*)it->second;
//      v->multiplyMatrix(m);
//      return true;
//    }
//  }
//
//#endif
//#ifdef JAVA_CODE
//  final Object[] uni = _uniformValues.values().toArray();
//  final Object[] uniNames = _uniformValues.keySet().toArray();
//  for (int i = 0; i < uni.length; i++) {
//    final String thisName =  (String) uniNames[i];
//    final GPUUniformValue uv = (GPUUniformValue) uni[i];
//    if ((thisName.compareTo(name) == 0) && (uv.getType() == GLType.glMatrix4Float()))
//    {
//      final GPUUniformValueMatrix4FloatStack v = (GPUUniformValueMatrix4FloatStack)uv;
//      v.multiplyMatrix(m);
//      return;
//    }
//  }
//#endif
//
//  ILogger::instance()->logError("CAN'T MULTIPLY UNLOADED MATRIX");
//  return false;
//
//}

bool GPUProgramState::setUniformMatrixValue(const std::string& name, const MutableMatrix44D& m, bool isTransform){
  GPUUniformValueMatrix4FloatTransform *uv = new GPUUniformValueMatrix4FloatTransform(m, isTransform);
  return setGPUUniformValue(name, uv);
}

void GPUProgramState::setAttributeEnabled(const std::string& name, bool enabled){
  //TODO: REMOVE FUNCTION
  if (!enabled){
    setAttributeDisabled(name);
  }
  
  //  attributeEnabledStruct ae;
  //  ae.value = enabled;
  //  ae.attribute = NULL;
  //
  //  _attributesEnabled[name] = ae;
}

void GPUProgramState::setAttributeDisabled(const std::string& name){
  setGPUAttributeValue(name, new GPUAttributeValueDisabled());
}

std::string GPUProgramState::description() const{
  std::string desc = "PROGRAM STATE\n==========\n";
  //TODO: IMPLEMENT
#ifdef C_CODE
  for(std::map<std::string, GPUUniformValue*> ::const_iterator it = _uniformValues.begin();
      it != _uniformValues.end();
      it++){
    
    std::string name = it->first;
    GPUUniformValue* v = it->second;
    
    desc += "Uniform " + name + ":\n";
    desc += v->description() + "\n";
  }
  
  for(std::map<std::string, GPUAttributeValue*> ::const_iterator it = _attributesValues.begin();
      it != _attributesValues.end();
      it++){
    
    std::string name = it->first;
    GPUAttributeValue* v = it->second;
    
    desc += "Attribute " + name + ":\n";
    desc += v->description() + "\n";
  }
#endif
  return desc;
}

std::vector<std::string>* GPUProgramState::getUniformsNames() const{
  
  if (_uniformNames == NULL){
    
    _uniformNames = new std::vector<std::string>();
    
#ifdef C_CODE
    for(std::map<std::string, GPUUniformValue*> ::const_iterator it = /*state->*/_uniformValues.begin();
        it != _uniformValues.end();
        it++){
      _uniformNames->push_back(it->first);
    }
#endif
    
#ifdef JAVA_CODE
    final Object[] uniNames = _uniformValues.keySet().toArray();
    for (int i = 0; i < uniNames.length; i++) {
      final String name = (String) uniNames[i];
      _uniformNames.add(name);
    }
#endif
    
  }
  return _uniformNames;
}

bool GPUProgramState::isLinkableToProgram(const GPUProgram& program) const{
#ifdef C_CODE
  if (program.getGPUAttributesNumber() != _attributesValues.size()){
    return false;
  }
  
  if (program.getGPUUniformsNumber()   != _uniformValues.size()){
    return false;
  }
  
  for(std::map<std::string, GPUUniformValue*> ::const_iterator it = _uniformValues.begin();
      it != _uniformValues.end();
      it++){
    if (program.getGPUUniform(it->first) == NULL){
      return false;
    }
  }
  
  for(std::map<std::string, GPUAttributeValue*> ::const_iterator it = _attributesValues.begin();
      it != _attributesValues.end();
      it++){
    if (program.getGPUAttribute(it->first) == NULL){
      return false;
    }
  }
  
  return true;
#endif
#ifdef JAVA_CODE
  
  if (program.getGPUAttributesNumber() != (_attributesValues.size())) {
    return false;
  }
  
  if (program.getGPUUniformsNumber() != _uniformValues.size()) {
    return false;
  }
  
  
  final Object[] uniNames = _uniformValues.keySet().toArray();
  for (int i = 0; i < uniNames.length; i++) {
    String thisName = (String) uniNames[i];
    if (program.getGPUUniform(thisName) == null) {
      return false;
    }
  }
  
  final Object[] attNames = _attributesValues.keySet().toArray();
  for (int i = 0; i < attNames.length; i++) {
    String thisName = (String) attNames[i];
    if (program.getGPUAttribute(thisName) == null) {
      return false;
    }
  }
  
  return true;
#endif
}