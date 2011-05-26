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

using std::string;
using std::ostringstream;

namespace sxml {

element::element(element *parent)
    : m_parent(parent),
      m_modified(true)
{}

element::element(const element &elem)
    : m_parent(elem.m_parent),
      m_children(elem.clone_children()),
      m_attributes(elem.m_attributes),
      m_name(elem.m_name),
      m_text(elem.m_text),
      m_cached(elem.m_cached),
      m_modified(elem.m_modified)
{}

element::element(const string &name, element *parent)
    : m_parent(parent),
      m_name(name),
      m_modified(true)
{}

element::~element()
{
    //cout << __FUNCTION__ << " " << m_name << endl;

    element_list::iterator ei = m_children.begin();
    for (; ei != m_children.end(); ++ei)
    {
        delete (*ei);
    }
}

string element::to_string(bool nice, int indent)
{
    if (!m_modified)
        return m_cached;

    string xmlstr;
    ostringstream xml(xmlstr);

    if (nice)
        xml << string(indent, ' ');

    xml << "<" << m_name;

    attribute_map::iterator ai = m_attributes.begin();
    for (; ai != m_attributes.end(); ++ai)
    {
        xml << " " << ai->first << "=\"" << ai->second << "\"";
    }

    if (m_children.empty() && m_text.empty())
    {
        xml << " />";
        return xml.str();
    }
    else if (nice)
    {
        xml << ">\n";
    }
    else
    {
        xml << ">";
    }

    if (!m_children.empty())
    {
        element_list::const_iterator ei = m_children.begin();
        for (; ei != m_children.end(); ++ei)
        {
            xml << (*ei)->to_string(nice, indent + 2);

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

element *element::add_child(element *child)
{
    m_children.push_back(child);
    m_modified = true;
    return this;
}

template<> element *element::set_text<>(const string &text)
{
    m_text = text;
    m_modified = true;
    return this;
}

template<> element *element::set_attr<>(const string &name,
                                        const string &value)
{
    m_attributes[name] = value;
    m_modified = true;
    return this;
}

element_list element::clone_children() const
{
    element_list clones;

    element_list::const_iterator ei = m_children.begin();
    for (; ei != m_children.end(); ++ei)
    {
        element *clone = new element(*(*ei));
        clones.push_back(clone);
    }

    return clones;
}

void element::set_modified(bool modified)
{
    m_modified = modified;

    if (modified)
    {
        if (m_parent != NULL)
            m_parent->set_modified(true);
    }
}

} // namespace sxml

