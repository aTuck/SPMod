/*  SPMod - SourcePawn Scripting Engine for Half-Life
 *  Copyright (C) 2018  SPMod Development Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "spmod.hpp"

meta_globals_t *gpMetaGlobals;
gamedll_funcs_t *gpGamedllFuncs;
mutil_funcs_t *gpMetaUtilFuncs;

// Core module definition
std::unique_ptr<SPModModule> gSPModModuleDef;

plugin_info_t Plugin_info =
{
    META_INTERFACE_VERSION,
    "SPMod",
    gSPModVersion,
    __DATE__,
    gSPModAuthor,
    "https://github.com/Amaroq7/SPMod",
    "SPMOD",
    PT_STARTUP,
    PT_ANYTIME
};

C_DLLEXPORT int Meta_Query(char *interfaceVersion [[maybe_unused]],
                            plugin_info_t **plinfo,
                            mutil_funcs_t *pMetaUtilFuncs)
{

    *plinfo = &Plugin_info;
    gpMetaUtilFuncs = pMetaUtilFuncs;

    return 1;
}

META_FUNCTIONS gMetaFunctionTable =
{
    nullptr,
    nullptr,
    GetEntityAPI2,
    GetEntityAPI2_Post,
    GetNewDLLFunctions,
    GetNewDLLFunctions_Post,
    GetEngineFunctions,
    GetEngineFunctions_Post
};

C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now [[maybe_unused]],
                            META_FUNCTIONS *pFunctionTable,
                            meta_globals_t *pMGlobals,
                            gamedll_funcs_t *pGamedllFuncs)
{
    gpMetaGlobals = pMGlobals;
    gpGamedllFuncs = pGamedllFuncs;

    // Have to be created before global object
    gSPModModuleDef = std::make_unique<SPModModule>(gCoreNatives);

    try
    {
        gSPGlobal = std::make_unique<SPGlobal>(GET_PLUGIN_PATH(PLID));
    }
    catch (const std::exception &e)
    {
        // Failed to initialize gSPGlobal, we gotta use this util func
        LOG_CONSOLE(PLID, "[SPMOD] %s", e.what());
        return 0;
    }

    auto &logSystem = gSPGlobal->getLoggerCore();
    {
        logSystem->LogConsoleCore(""); // Empty line
        logSystem->LogConsoleCore("   SPMod Copyright (c) 2018 ", gSPModAuthor);
        logSystem->LogConsoleCore("   This program comes with ABSOLUTELY NO WARRANTY; for details type `spmod gpl'.");
        logSystem->LogConsoleCore("   This is free software, and you are welcome to redistribute it");
        logSystem->LogConsoleCore("   under certain conditions; type `spmod gpl' for details.");
        logSystem->LogConsoleCore(""); // Empty line
        logSystem->LogConsoleCore("SPMod ", gSPModVersion, ", API ", SPMOD_API_VERSION);
        logSystem->LogConsoleCore("SPMod build: ", __TIME__, " ", __DATE__);
    }

    if (!initRehldsApi())
    {
        logSystem->LogErrorCore("SPMod requires to have ReHLDS installed!");
        return 0;
    }

    memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));

    return 1;
}

C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now [[maybe_unused]],
                            PL_UNLOAD_REASON reason [[maybe_unused]])
{
    gSPGlobal->getPluginManagerCore()->detachPlugins();
    gSPGlobal->getForwardManagerCore()->clearForwards();

    return 1;
}
