/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <http://www.natron.fr/>,
 * Copyright (C) 2016 INRIA and Alexandre Gauthier-Foichat
 *
 * Natron is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Natron is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Natron.  If not, see <http://www.gnu.org/licenses/gpl-2.0.html>
 * ***** END LICENSE BLOCK ***** */

#ifndef Engine_AppInstanceWrapper_h
#define Engine_AppInstanceWrapper_h

// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****

#include "Global/Macros.h"

/**
 * @brief Simple wrap for the AppInstance class that is the API we want to expose to the Python
 * Engine module.
 **/


CLANG_DIAG_OFF(deprecated)
CLANG_DIAG_OFF(uninitialized)
#include <QtCore/QCoreApplication>
CLANG_DIAG_ON(deprecated)
CLANG_DIAG_ON(uninitialized)

#include "Engine/PyNode.h"
#include "Engine/EngineFwd.h"

NATRON_NAMESPACE_ENTER;
NATRON_PYTHON_NAMESPACE_ENTER;

class AppSettings
{
public:

    AppSettings(const boost::shared_ptr<Settings>& settings);

    Param* getParam(const QString& scriptName) const;
    std::list<Param*> getParams() const;

    void saveSettings();

    void restoreDefaultSettings();

private:

    boost::shared_ptr<Settings> _settings;
};


class App
    : public Group
{
    Q_DECLARE_TR_FUNCTIONS(App)

private:
    AppInstance* _instance;

public:


    App(AppInstance* instance);

    virtual ~App() {}

    int getAppID() const;

    AppInstance* getInternalApp() const;

    /**
     * @brief Creates a new instance of the plugin identified by the given pluginID.
     * @param majorVersion If different than -1, it will try to load a specific major version
     * of the plug-in, otherwise it will default to the greatest major version found for that plug-in.
     * @param group If not NULL, this should be a pointer to a group node where-in to insert the newly created effect.
     * If NULL, the newly created node will be inserted in the project's root.
     **/
    Effect* createNode(const QString& pluginID,
                       int majorVersion = -1,
                       Group* group = 0) const;
    Effect* createReader(const QString& filename,
                         Group* group = 0) const;
    Effect* createWriter(const QString& filename,
                         Group* group = 0) const;

    int timelineGetTime() const;

    int timelineGetLeftBound() const;

    int timelineGetRightBound() const;

    void addFormat(const QString& formatSpec);

    void render(Effect* writeNode, int firstFrame, int lastFrame, int frameStep = 1);
    void render(const std::list<Effect*>& effects, const std::list<int>& firstFrames, const std::list<int>& lastFrames, const std::list<int>& frameSteps);

    Param* getProjectParam(const QString& name) const;

    void writeToScriptEditor(const QString& message);

    bool saveProject(const QString& filename);
    bool saveProjectAs(const QString& filename);
    bool saveTempProject(const QString& filename);
    App* loadProject(const QString& filename);

    ///Close the current project but keep the window
    bool resetProject();

    ///Reset + close window, quit if last window
    bool closeProject();

    ///Opens a new window
    App* newProject();
    std::list<QString> getViewNames() const;

    void addProjectLayer(const ImageLayer& layer);

protected:

    void renderInternal(bool forceBlocking, Effect* writeNode, int firstFrame, int lastFrame, int frameStep);
    void renderInternal(bool forceBlocking, const std::list<Effect*>& effects, const std::list<int>& firstFrames, const std::list<int>& lastFrames,
                        const std::list<int>& frameSteps);

    boost::shared_ptr<NodeCollection> getCollectionFromGroup(Group* group) const;
};

NATRON_PYTHON_NAMESPACE_EXIT;
NATRON_NAMESPACE_EXIT;

#endif // Engine_AppInstanceWrapper_h
