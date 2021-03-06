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

#pragma once

#include "spmod.hpp"

class Forward;

class Plugin final : public IPlugin
{
public:

    static constexpr uint32_t FIELD_NAME = 0;
    static constexpr uint32_t FIELD_VERSION = 1;
    static constexpr uint32_t FIELD_AUTHOR = 2;
    static constexpr uint32_t FIELD_URL = 3;

    Plugin(size_t id,
            std::string_view identity,
            const fs::path &path);

    Plugin() = delete;
    ~Plugin();

    // IPlugin
    const char *getName() const override
    {
        return m_name.c_str();
    }
    const char *getVersion() const override
    {
        return m_version.c_str();
    }
    const char *getAuthor() const override
    {
        return m_author.c_str();
    }
    const char *getUrl() const override
    {
        return m_url.c_str();
    }
    const char *getIndentity() const override
    {
        return m_identity.c_str();
    }
    const char *getFileName() const override
    {
        return m_filename.c_str();
    }
    size_t getId() const override
    {
        return m_id;
    }
    SourcePawn::IPluginRuntime *getRuntime() const override
    {
        return m_runtime;
    }
    IForward *createForward(const char *name,
                            IModuleInterface *owner,
                            size_t params,
                            ...) const override;
    IForward *createForward(const char *name,
                            IPlugin *owner,
                            size_t params,
                            ...) const override;

    // Plugin
    std::string_view getNameCore() const
    {
        return m_name;
    }
    std::string_view getVersionCore() const
    {
        return m_version;
    }
    std::string_view getAuthorCore() const
    {
        return m_author;
    }
    std::string_view getUrlCore() const
    {
        return m_url;
    }
    std::string_view getIndentityCore() const
    {
        return m_identity;
    }
    std::string_view getFileNameCore() const
    {
        return m_filename;
    }
    std::shared_ptr<Forward> createForwardCore(std::string_view name,
                                                fwdOwnerVariant owner,
                                                fwdInitParamsList params) const;

private:
    std::shared_ptr<Forward> _createForwardVa(std::string_view name,
                                                fwdOwnerVariant owner,
                                                va_list paramsList,
                                                size_t paramsnum) const;
    std::shared_ptr<Forward> _createForward(std::string_view name,
                                            fwdOwnerVariant owner,
                                            fwdParamTypeList paramlist,
                                            size_t paramsnum) const;

    SourcePawn::IPluginRuntime *m_runtime;
    std::string m_identity;
    std::string m_filename;
    std::string m_name;
    std::string m_version;
    std::string m_author;
    std::string m_url;
    size_t m_id;
};

class PluginMngr final : public IPluginMngr
{
public:
    PluginMngr() = default;
    ~PluginMngr() = default;

    // IPluginMngr
    size_t getPluginsNum() const override
    {
        return m_plugins.size();
    }
    IPlugin *getPlugin(size_t index) override
    {
        return getPluginCore(index).get();
    }
    IPlugin *getPlugin(const char *name) override
    {
        return getPluginCore(name).get();
    }
    IPlugin *getPlugin(SourcePawn::IPluginContext *ctx) override
    {
        return getPluginCore(ctx).get();
    }
    IPlugin *loadPlugin(const char *name,
                        char *error,
                        size_t size) override;

    // PluginMngr
    const auto &getPluginsList() const
    {
        return m_plugins;
    }
    void detachPlugins()
    {
        m_plugins.clear();
    }
    void setPluginPrecache(bool canprecache)
    {
        m_canPluginsPrecache = canprecache;
    }
    bool canPluginPrecache()
    {
        return m_canPluginsPrecache;
    }
    std::shared_ptr<Plugin> loadPluginCore(std::string_view name,
                                            std::string *error);

    std::shared_ptr<Plugin> getPluginCore(size_t index);
    std::shared_ptr<Plugin> getPluginCore(std::string_view name);
    std::shared_ptr<Plugin> getPluginCore(SourcePawn::IPluginContext *ctx);
    size_t loadPlugins();

private:
    std::shared_ptr<Plugin> _loadPlugin(const fs::path &path,
                                        std::string *error);
    std::unordered_map<std::string, std::shared_ptr<Plugin>> m_plugins;

    // Allow plugins to precache
    bool m_canPluginsPrecache;
};
