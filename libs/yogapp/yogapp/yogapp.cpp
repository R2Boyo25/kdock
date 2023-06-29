// #include "yogapp.hpp"

/*
yogapp::Node::Node() {
  this->ygnode = YGNodeNew();
}

yogapp::Node::Node(YGNodeRef ygnode) {
  this->ygnode = ygnode;
}

yogapp::Node::~Node() {
  YGNodeFreeRecursive(this->ygnode);
}

YGNodeRef yogapp::Node::asNodeRef() {
  return this->ygnode;
}

yogapp::Node yogapp::Node::clone() {
  return YGNodeClone(this->ygnode);
}

yogapp::Node yogapp::Node::getChild(uint32_t index) {
  return YGNodeGetChild(this->ygnode, index);
}

void yogapp::Node::removeChild(yogapp::Node child) {
  YGNodeRemoveChild(this->ygnode, child.asNodeRef());
}

void yogapp::Node::removeAllChildren() {
  YGNodeRemoveAllChildren(this->ygnode);
}

yogapp::Node yogapp::Node::getOwner() {
  return YGNodeGetOwner(this->ygnode);
}

yogapp::Node yogapp::Node::getParent() {
  return YGNodeGetParent(this->ygnode);
}

void yogapp::Node::setIsReferenceBaseline(bool value) {
  YGNodeSetIsReferenceBaseline(this->ygnode, value);
}

bool yogapp::Node::getIsReferenceBaseline() {
  return YGNodeIsReferenceBaseline(this->ygnode);
}

void yogapp::Node::calculateLayout(float available_width, float available_height, YGDirection owner_direction) {
  YGNodeCalculateLayout(this->ygnode, available_width, available_height, owner_direction);
}

void yogapp::Node::markDirty() {
  YGNodeMarkDirty(this->ygnode);
}

void yogapp::Node::recursivelyMarkDirty() {
  YGNodeMarkDirtyAndPropagateToDescendants(this->ygnode);
}

void yogapp::Node::print(YGPrintOptions options) {
  YGNodePrint(this->ygnode, options);
}

void yogapp::Node::copyStyle(Node from) {
  YGNodeCopyStyle(this->ygnode, ygnode);
}
*/
