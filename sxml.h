/*
 * This file is part of sxml - stupid XML generator
 *
 * Copyright (c) 2011 Oskari Timperi
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute
 * it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 * not claim that you wrote the original software. If you use this
 * software in a product, an acknowledgment in the product documentation
 * would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must
 * not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 */

#ifndef SXML_H
#define SXML_H

#include <string>
#include <sstream>
#include <map>
#include <vector>

namespace sxml {

class node;

typedef std::vector<node *> node_list;

//! Base class for nodes, e.g. an element, a comment, text, ...
class node
{
    public:

    //! Constructs an empty node. If parent is not NULL, the constructed node
    //! is added to the given parent as a child.
    explicit node(node *parent = NULL);

    //! Copy constructor.
    node(const node &n);

    //! Destructor. Frees the children of this node.
    virtual ~node();

    //! Adds a child node to this node. Takes the ownership of the given
    //! node. Returns a pointer to the child.
    node *add_child(node *child);

    //! Returns a string representation of this node. Derived classes should
    //! implement this.
    virtual std::string to_string(bool nice , int indent = 0) const;

    protected:

    //! Returns a list of children in this node.
    const node_list &children() const;

    //! Clones this node. The cloned node has clones of all the children of
    //! the original. The cloned node doesn't have a parent.
    virtual node *clone();

    //! Clone the children of this node. The children do not have a parent set.
    node_list clone_children() const;

    //! Is this node modified? Derived classes can use this information for
    //! caching.
    bool modified() const;

    //! Set the modified status of this node. If modified is true and the node
    //! has a parent, set_modified() also calls the parents set_modified().
    void set_modified(bool modified) const;

    private:

    //! Pointer to the parent node.
    node *m_parent;

    //! List of the children.
    node_list m_children;

    //! Is this node modified.
    mutable bool m_modified;
};

//! A node used for comments.
class comment: public node
{
    public:

    explicit comment(node *parent = NULL);
    explicit comment(const std::string &text, node *parent = NULL);

    virtual std::string to_string(bool nice = false, int indent = 0) const;

    private:

    std::string m_text;
};

class element: public node
{
    public:

    //! Constructs an empty element (i.e., no name,
    //! children, attributes) with the specified parent.
    explicit element(node *parent = NULL);

    //! Constructs an element with the specified name and parent.
    explicit element(const std::string &name, node *parent = NULL);

    //! Copy constructor.
    element(const element &elem);

    virtual ~element();

    //! Creates a textual representation of the element. If nice
    //! is true, the returned string is formatted with indentations
    //! and newlines.
    virtual std::string to_string(bool nice = false, int indent = 0) const;

    //! Adds a child with the given tag to this element. Returns a pointer
    //! to the child.
    element *add_element(const std::string &tag);

    //! Set the text for this element. An element can either have text
    //! or children. If an element has both, children take precedence
    //! when calling to_string(). Returns a pointer to this element.
    template<class T> element *set_text(const T &text);

    //! Set an attribute for this element with the specified name and
    //! value. Returns a pointer to this element.
    template<class T> element *set_attr(const std::string &name,
                                        const T &value);

    private:

    typedef std::map<std::string, std::string> attribute_map;

    attribute_map m_attributes;

    std::string m_name;
    std::string m_text;

    mutable std::string m_cached;
};

template<class T> element *element::set_text(const T &text)
{
    std::string s;
    std::stringstream ss(s);
    ss << text;

    return set_text<std::string>(ss.str());
}

template<> element *element::set_text<>(const std::string &text);

template<class T> element *element::set_attr(const std::string &name,
                                             const T &value)
{
    std::string s;
    std::stringstream ss(s);
    ss << value;

    return set_attr<std::string>(name, ss.str());
}

template<> element *element::set_attr(const std::string &name,
                                      const std::string &value);

} // namespace sxml

#endif // SXML_H

