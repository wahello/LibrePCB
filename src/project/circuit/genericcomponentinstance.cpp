/*
 * EDA4U - Professional EDA for everyone!
 * Copyright (C) 2013 Urban Bruhin
 * http://eda4u.ubruhin.ch/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/

#include <QtCore>
#include <QtWidgets>
#include "genericcomponentinstance.h"
#include "../../common/exceptions.h"
#include "circuit.h"
#include "../project.h"
#include "../library/projectlibrary.h"
#include "gencompsignalinstance.h"

namespace project {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

GenericComponentInstance::GenericComponentInstance(Circuit& circuit,
                                                   const QDomElement& domElement)
                                                   throw (Exception) :
    QObject(0), mCircuit(circuit), mDomElement(domElement)
{
    mUuid = mDomElement.attribute("uuid");
    if(mUuid.isNull())
    {
        throw RuntimeError(__FILE__, __LINE__, mDomElement.attribute("uuid"),
            QString(tr("Invalid generic component instance UUID: \"%1\""))
            .arg(mDomElement.attribute("uuid")));
    }

    mName = mDomElement.attribute("name");
    if(mName.isEmpty())
    {
        throw RuntimeError(__FILE__, __LINE__, mUuid.toString(),
            QString(tr("Name of generic component instance \"%1\" is empty!"))
            .arg(mUuid.toString()));
    }

    mGenComp = mCircuit.getProject().getLibrary().getGenericComponent(
                   mDomElement.attribute("generic_component"));
    if (!mGenComp)
    {
        throw RuntimeError(__FILE__, __LINE__, mDomElement.attribute("generic_component"),
            QString(tr("The generic component with the UUID \"%1\" does not exist in the "
            "project's library!")).arg(mDomElement.attribute("generic_component")));
    }

    QDomElement tmpNode; // for temporary use...

    // load all signal instances
    tmpNode = mDomElement.firstChildElement("signal_mapping").firstChildElement("map");
    while (!tmpNode.isNull())
    {
        GenCompSignalInstance* signal = new GenCompSignalInstance(*this, tmpNode);
        mSignals.insert(signal->getCompSignalUuid(), signal);
        tmpNode = tmpNode.nextSiblingElement("map");
    }
    // TODO: compare the signal count with the generic component object of the library
}

GenericComponentInstance::~GenericComponentInstance() noexcept
{
    qDeleteAll(mSignals);       mSignals.clear();
}

/*****************************************************************************************
 *  Setters
 ****************************************************************************************/

void GenericComponentInstance::setName(const QString& name) throw (Exception)
{
    if(name.isEmpty())
    {
        throw RuntimeError(__FILE__, __LINE__, QString(),
            tr("The new component name must not be empty!"));
    }

    mDomElement.setAttribute("name", name);
    mName = name;
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

void GenericComponentInstance::addToCircuit(bool addNode, QDomElement& parent) throw (Exception)
{
    if (addNode)
    {
        if (parent.nodeName() != "generic_component_instances")
            throw LogicError(__FILE__, __LINE__, parent.nodeName(), tr("Invalid node name!"));

        if (parent.appendChild(mDomElement).isNull())
            throw LogicError(__FILE__, __LINE__, QString(), tr("Could not append DOM node!"));
    }

    foreach (GenCompSignalInstance* signal, mSignals)
        signal->addToCircuit();
}

void GenericComponentInstance::removeFromCircuit(bool removeNode, QDomElement& parent) throw (Exception)
{
    if (removeNode)
    {
        if (parent.nodeName() != "generic_component_instances")
            throw LogicError(__FILE__, __LINE__, parent.nodeName(), tr("Invalid node name!"));

        if (parent.removeChild(mDomElement).isNull())
            throw LogicError(__FILE__, __LINE__, QString(), tr("Could not remove node from DOM tree!"));
    }

    foreach (GenCompSignalInstance* signal, mSignals)
        signal->removeFromCircuit();
}

/*****************************************************************************************
 *  Static Methods
 ****************************************************************************************/

GenericComponentInstance* GenericComponentInstance::create(Circuit& circuit,
                                                           QDomDocument& doc,
                                                           const QUuid& genericComponent,
                                                           const QString& name)
                                                           throw (Exception)
{
    QDomElement node = doc.createElement("instance");
    if (node.isNull())
        throw LogicError(__FILE__, __LINE__, QString(), tr("Could not create DOM node!"));

    // fill the new QDomElement with all the needed content
    node.setAttribute("uuid", QUuid::createUuid().toString()); // generate random UUID
    node.setAttribute("name", name);
    node.setAttribute("generic_component", genericComponent.toString());

    // create and return the new GenericComponentInstance object
    return new GenericComponentInstance(circuit, node);
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace project