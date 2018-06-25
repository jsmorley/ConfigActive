# PluginConfigActive

Creates the ConfigActive.dll plugin for Rainmeter.

Returns a number value of `1` if a named config is active / loaded, and `-1` if not.
Returns a string value of the skin .ini variant name of the named config.

##Checking active status of a skin

###Using a measure option

**ConfigName**
The name of a config you wish to test. This is case-insensitive.

Example:

```
[MeasureName]
Measure=Plugin
Plugin=ConfigActive
ConfigName=Illustro\System
```

**- The number value of the measure will be 1 if the config is active and -1 if not.**
**- The string value of the measure will be the currently running skin .ini file name.**

Note: All other measure specific options are ignored when ConfigName is set.

###Using inline plugin section variables

**[&MeasureName:IsActive(ConfigName)]**
The section variable will be resolved to `1` if the config is active and `-1` if not.

Example `IfCondition=[&MeasureName:IsActive(Illustro\System)] = 1`

Note: `DynamicVariables=1` must be set on the measure or meter where this is used.

**[&MeasureName:ConfigVariantName(ConfigName)]**
The section variable will be resolved to the name of currently running skin .ini file name.

Example `IfTrueAction=[!SetOption SomeMeter Text "Variant [&MeasureName:ConfigVariantName(Illustro\System)] is running."]`

Note: DynamicVariables=1 must be set on the measure or meter where this is used.

##Getting a count of running skins

###Using a measure option

**TypeSkin**
In addition to the basic functionality supported by the ConfigName option, the plugin will obtain a "count" of the number of active configs. This will create a numbered "index" of all active configs, ordered by the current z-position (front to back) of the skin windows.

This count can be returned as the number value by setting `Type=Count` on the measure.

Type can also used in conjunction with `Index`, to return either the config name or skin .ini file name of the config referenced with Index. This is done with `Type=Config` or `Type=Skin` and `Index=x`.

Note: The intent of this is to be able to use Count, and then, perhaps in Lua, iterate through Index from 1 to the value of Count, to obtain a list of configs and / or skins currently active. In and of itself, Index is not terribly useful, as the position of any given config in this list is really unreliable on any given user's system at any given time.

**Index1**
Used in conjunction with Type=Config or Type=Skin to return ether the config name or skin .ini name for a particular indexed config.

###Using inline plugin section variables

**[&MeasureName:LoadedCount()]**
The section variable will be resolved to the number of currently active configs.

Example `Formula=[&MeasureName:LoadedCount()]` 

More details and examples of use at [Rainmeter Forums](https://forum.rainmeter.net/viewtopic.php?p=149824)
