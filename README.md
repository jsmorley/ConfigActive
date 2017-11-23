# PluginConfigActive

Creates the ConfigActive.dll plugin for Rainmeter.

Returns `1` if a named config is active / loaded, and `-1` if not.

Build in conjunction with the Rainmeter Plugin SDK from [Rainmeter Plugin SDK](https://github.com/rainmeter/rainmeter-plugin-sdk)  
Documentation for the SDK at [Developers Documentation](https://docs.rainmeter.net/developers/#CreatePlugin)

## Options

### ConfigName
`ConfigName=illustro\System` 

## Inline Section Variable Functions

### IsActive(`config name`)

`[&MeasureName:IsActive(illustro\System)]`  
The inline section variable will be replaced where used with `1` if the named config is active / loaded and `-1` if not.  
`DynamnicVariables=1` must be set on the measure or meter where this is used.  
Do not enclose the config name in quotes.  
