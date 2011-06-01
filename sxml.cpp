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

#include "sxml.h"

#include <sstream>

using std::string;
using std::ostringstream;

namespace sxml {

node::node(node *parent)
    : m_parent(parent),
      m_modified(true)
{
    if (parent != NULL)
    {
        parent->add_child(this);
    }
}

node::node(const node &n)
    : m_parent(NULL),
      m_children(n.m_children),
      m_modified(n.m_modified)
{}

node::~node()
{
    node_list::iterator i = m_children.begin();
    for (; i != m_children.end(); ++i)
    {
        delete (*i);
    }
}

node *node::add_child(node *child)
{
    child->m_parent = this;
    m_children.push_back(child);
    set_modified(true);
    return child;
}

string node::to_string(bool nice , int indent) const
{
    nice = nice;
    indent = indent;
    return string();
}

const node_list &node::children() const
{
    return m_children;
}

node *node::clone()
{
    node *n = new node;
    n->m_children = clone_children();
    n->m_modified = m_modified;
    return n;
}

node_list node::clone_children() const
{
    node_list clones;

    node_list::const_iterator i = m_children.begin();
    for (; i != m_children.end(); ++i)
    {
        clones.push_back((*i)->clone());
    }

    return clones;
}

bool node::modified() const
{
    return m_modified;
}

void node::set_modified(bool modified) const
{
    m_modified = modified;

    if (modified && m_parent != NULL)
    {
        m_parent->set_modified(true);
    }
}

// *******************************************************************

comment::comment(node *parent)
    : node(parent)
{}

comment::comment(const string &text, node *parent)
    : node(parent)
{
    m_text = text;
}

string comment::to_string(bool nice, int indent) const
{
    string result;

    if (nice)
        result = string(indent, ' ');

    result += string("<!-- ") + m_text + string(" -->");

    return result;
}

// *******************************************************************

element::element(node *parent)
    : node(parent)
{}

element::element(const element &elem)
    : node(elem),
      m_attributes(elem.m_attributes),
      m_name(elem.m_name),
      m_text(elem.m_text),
      m_cached(elem.m_cached)
{}

element::element(const string &name, node *parent)
    : node(parent),
      m_name(name)
{}

element::~element()
{
}

string element::to_string(bool nice, int indent) const
{
    if (!modified())
        return m_cached;

    string xmlstr;
    ostringstream xml(xmlstr);

    if (nice)
        xml << string(indent, ' ');

    xml << "<" << m_name;

    attribute_map::const_iterator ai = m_attributes.begin();
    for (; ai != m_attributes.end(); ++ai)
    {
        xml << " " << ai->first << "=\"" << ai->second << "\"";
    }

    if (children().empty() && m_text.empty())
    {
        xml << " />";
        return xml.str();
    }
    else
    {
        xml << (nice ? ">\n" : ">");
    }

    if (!children().empty())
    {
        node_list::const_iterator ni = children().begin();
        for (; ni != children().end(); ++ni)
        {
            xml << (*ni)->to_string(nice, indent + 2);

            if (nice)
                xml << "\n";
        }
    }
    else if (!m_text.empty())
    {
        if (nice)
            xml << string(indent + 2, ' ');

        xml << m_text << (nice ? "\n" : "");
    }

    if (nice)
        xml << string(indent, ' ');

    xml << "</" << m_name << ">";

    m_cached = xml.str();

    set_modified(false);

    return xml.str();
}

element *element::add_element(const std::string &tag)
{
    return new element(tag, this);
}

template<> element *element::set_text<>(const string &text)
{
    m_text = text;
    set_modified(true);
    return this;
}

template<> element *element::set_attr<>(const string &name,
                                        const string &value)
{
    m_attributes[name] = value;
    set_modified(true);
    return this;
}

} // namespace sxml

