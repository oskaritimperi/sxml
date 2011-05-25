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

namespace sxml {

element::element()
{}

element::element(const element &elem)
    : m_children(elem.m_children),
      m_attributes(elem.m_attributes),
      m_name(elem.m_name),
      m_text(elem.m_text)
{}

element::element(const std::string &name)
    : m_name(name)
{}

std::string element::to_string(bool nice, int indent) const
{
    std::string xmlstr;
    std::ostringstream xml(xmlstr);
    
    if (nice)
        xml << std::string(indent, ' ');
    
    xml << "<" << m_name;
    
    attribute_map::const_iterator ai = m_attributes.begin();
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
            xml << ei->to_string(nice, indent + 2);
            
            if (nice)
                xml << "\n";
        } 
    }
    else if (!m_text.empty())
    {
        if (nice)
            xml << std::string(indent + 2, ' ');
        
        xml << m_text << (nice ? "\n" : "");
    }
    
    if (nice)
        xml << std::string(indent, ' ');
        
    xml << "</" << m_name << ">";
    
    return xml.str();
}

element &element::add_child(const element &child)
{
    m_children.push_back(child);
    return *this;
}

template<> element &element::set_text<>(const std::string &text)
{
    m_text = text;
    return *this;
}

template<> element &element::set_attr<>(const std::string &name,
    const std::string &value)
{
    m_attributes[name] = value;
    return *this;
}

} // namespace sxml

