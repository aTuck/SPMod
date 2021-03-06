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

class SPModModule final : public IModuleInterface
{
public:
    SPModModule() = delete;
    ~SPModModule() = default;

    SPModModule(sp_nativeinfo_t *natives)
    {
        this->m_natives = natives;
        
        size_t paramsNum = 0;
        while ((natives + paramsNum)->func)
            ++paramsNum;

        this->m_nativesNum = paramsNum;
    }

    const char *getName() const override
    {
        return "spmod";
    }
};

extern std::unique_ptr<SPModModule> gSPModModuleDef;
