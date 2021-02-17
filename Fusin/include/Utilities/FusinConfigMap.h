#ifndef _FUSIN_CONFIG_MAP_H
#define _FUSIN_CONFIG_MAP_H

#include "FusinPrerequisites.h"
#include <map>

namespace Fusin
{
    template<class _T>
    _T getCfgValue(const std::map<String, String>& config, String key, const _T def)
    {
		std::map<String, String>::const_iterator cfgIt = config.find(key);
        if (cfgIt != config.cend())
        {
		    StringStream ss;
            ss << cfgIt->second;
            _T res;
            ss >> res;
            return res;
        }
        else
            return def;
    }

    template<>
    bool getCfgValue<bool>(const std::map<String, String>& config, String key, const bool def);
}

#endif