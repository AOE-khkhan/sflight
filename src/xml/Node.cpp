
#include "sflight/xml/Node.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace sflight {
namespace xml {

Node::Node(const std::string& tagName) : name(tagName)
{}

Node::Node(const std::string& tagName, const std::string& text) : name(tagName), text(text)
{}

Node::Node(const Node& src)
{
   name = src.getTagName();
   text = src.getText();

   std::size_t cnt{src.getAttributeCount()};
   std::string* arry{new std::string[cnt]};
   src.getAttributeNames(arry);

   for (std::size_t i = 0; i < cnt; i++) {
      putAttribute(arry[i], src.getAttribute(arry[i]));
   }

   cnt = src.getChildCount();

   for (std::size_t i = 0; i < cnt; i++) {
      Node* tmp{new Node(*src.getChild(i))};
      addChild(tmp);
   }

   delete[] arry;
}

Node::~Node()
{
   for (std::size_t i = childList.size() - 1; i >= 0; i--) {
      delete childList[i];
   }
}

std::string Node::getTagName() const { return name; }

void Node::setTagName(const std::string& x) { name = x; }

Node* Node::addChild(const std::string& x)
{
   Node* child{new Node(x)};
   return addChild(child);
}

Node* Node::addChild(Node* const x)
{
   childList.push_back(x);
   x->setParent(this);
   return x;
}

std::size_t Node::getChildCount() const { return childList.size(); }

Node* Node::getChild(const std::size_t index) const
{
   if (index < getChildCount())
      return childList[index];

   return 0;
}

//
// Returns the first child encountered with specified name, or null
// if none is found.  To find a nested child, specify the childname
// as tags separated by "/"
//
Node* Node::getChild(const std::string& x) const
{
   std::string childName{x};
   const Node* tmp{this};

   const std::size_t splitPt{childName.find_first_of("/")};
   std::string tail;

   if (splitPt != std::string::npos) {
      tail = childName.substr(splitPt + 1);
      childName = childName.substr(0, splitPt);
   }

   for (std::size_t i = 0; i < tmp->childList.size(); i++) {
      if (tmp->childList[i]->getTagName() == childName) {
         if (tail != "")
            return childList[i]->getChild(tail);
         else
            return childList[i];
      }
   }
   return nullptr;
}

//
// Returns a vector containing all children encountered with specified name, or null
// if none are found.
//
std::vector<Node*> Node::getChildren(const std::string& x) const
{
   std::string childName{x};
   std::vector<Node*> list;

   const Node* tmp{this};

   const std::size_t splitPt{childName.find_first_of("/")};
   std::string tail;

   if (splitPt != std::string::npos) {
      tail = childName.substr(splitPt + 1);
      childName = childName.substr(0, splitPt);
   }

   for (std::size_t i = 0; i < tmp->childList.size(); i++) {
      if (tmp->childList[i]->getTagName() == childName) {
         if (tail != "") {
            std::vector<Node*> sublist{childList[i]->getChildren(tail)};
            for (std::size_t j = 0; j < sublist.size(); j++) {
               list.push_back(sublist[j]);
            }
         } else {
            list.push_back(childList[i]);
         }
      }
   }

   return list;
}

void Node::putAttribute(std::string name, std::string val)
{
   attrMap.insert(std::pair<std::string, std::string>(name, val));
}

std::string Node::getAttribute(const std::string& name) const
{
   if (attrMap.count(name) == 1) {
      return attrMap.at(name);
   }
   return 0;
}

void Node::getAttributeNames(std::string* const storeArray) const
{
   std::map<std::string, std::string>::const_iterator iter;
   int i{};

   for (iter = attrMap.begin(); iter != attrMap.end(); ++iter) {
      storeArray[i] = iter->first;
      i++;
   }
}

std::size_t Node::getAttributeCount() const { return attrMap.size(); }

std::string Node::getText() const { return text; }

void Node::setText(const std::string& x) { text = x; }

Node* Node::getParent() const { return parentNode; }

void Node::setParent(Node* const x) { parentNode = x; }

bool Node::remove(Node* const node)
{
   std::vector<Node*>::iterator it;

   for (it = childList.begin(); it < childList.end(); it++) {

      if (*it == node) {
         childList.erase(it);
         return true;
      }
   }
   return false;
}

std::string Node::toString() const
{
   std::string ret{"<" + getTagName() + " "};
   std::string* attrNames{new std::string[getAttributeCount()]};
   getAttributeNames(attrNames);

   for (std::size_t i = 0; i < getAttributeCount(); i++) {
      ret += (attrNames[i] + "=" + getAttribute(attrNames[i]) + " ");
   }
   ret += ">";

   for (std::size_t i = 0; i < childList.size(); i++) {
      ret += "\n";
      ret += childList[i]->toString();
   }

   if (getText() != "") {
      ret += ("\n  " + getText());
   }

   ret += "\n</" + getTagName() + " ";
//   ret += "\n</" + getTagName() + " " + "\n";   // mistake in orig code
   delete[] attrNames;
   return ret;
}
}
}
