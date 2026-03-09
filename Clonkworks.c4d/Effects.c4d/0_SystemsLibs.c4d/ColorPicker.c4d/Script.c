/*-- Color Picker --*/

/*
	There are two ways to use this object:

	if you're already writing an #appendto, use this:
	OpenColorPicker(object clonk, object target, int start_color, bool has_alpha);
	  -> target->~ColorPickerCallback(color, clonk);

	otherwise, use this:
	OpenColorPickerCmd(object clonk, string command, int start_color, bool has_alpha);
	  -> eval(Format(command, color));

	the latter is more universal, but also more clunky, as it requires string formatting like this:
	Format("SetClrModulation(%s, Object(%d))", "%d", ObjectNumber(target));
*/

#strict 2

static const CLRP_NoCount = 12345678; // C4MN_Item_NoCount
static const CLRP_DefaultValueRange = 8; // 9 values, 8 steps

local r, g, b, a;
local clonk;
local target;
local command;
local has_alpha;
local use_command;
local value_range; // default is 8

func Color() 
{
	return ColorOf(r, g, b, a);
}

func ColorOf(int r, int g, int b, int a)
{
	var result = RGBa
	(
		IndexToByte(AdjustChannel(r)),
		IndexToByte(AdjustChannel(g)),
		IndexToByte(AdjustChannel(b)),
		IndexToByte(AdjustChannel(a))
	);
	
	if (!result)
	{
		return 0x010101;
	}	
	return result;
}

func ByteToIndex(int value)
{
	// do not round down, but use actual rounding for nearest color
	var stretched = value * value_range;
	return (stretched % 255 > 127) + stretched / 255;	
}

func IndexToByte(int value)
{
	var stretched = value * 255;
	return (stretched % value_range > value_range / 2) + stretched / value_range;
}

func ChannelOf(int value)
{
	AdjustChannel(value) * 255 / value_range;
}

func AdjustChannel(int value, int delta)
{
	return BoundBy(value + delta, 0, value_range);
}

func ChangeRed(int delta, int index)
{
	r = AdjustChannel(r, delta);	
	KeepMenu(index);
}

func ChangeGreen(int delta, int index)
{
	g = AdjustChannel(g, delta);
	KeepMenu(index);
}

func ChangeBlue(int delta, int index)
{
	b = AdjustChannel(b, delta);
	KeepMenu(index);
}

func ChangeAlpha(int delta, int index)
{
	a = AdjustChannel(a, delta);
	KeepMenu(index);
}

func ApplyColor()
{
	var result;
	if (use_command)
	{
		var cmd = Format(command, Color());
		result = eval(cmd);
	}
	else if (target)
	{
		result = target->~ColorPickerCallback(Color(), clonk);
	}
	RemoveObject(this);
	return result;
}

func ToSpecificColor(int color, int index)
{
	r = ByteToIndex(GetRGBaValue(color, 1));
	g = ByteToIndex(GetRGBaValue(color, 2));
	b = ByteToIndex(GetRGBaValue(color, 3));
	a = ByteToIndex(GetRGBaValue(color, 0));
	KeepMenu(index);
}

func CommandString(string function, int delta, int index)
{
	return Format("%s(%d, %d)", function, delta, index);
}

func AddChannelItem(string name, string function, int delta, int index, int color, bool enabled)
{
	AddMenuItem(name, CommandString(function, delta, index), CLRP, clonk, CLRP_NoCount, 0, "", C4MN_Add_ImgColor, color);
}

func AddMenuSpacers(int count)
{
	for (var i = 0; i < count; ++i)
	{
		AddMenuItem("", "", 0, clonk, CLRP_NoCount);
	}
}

func OpenMenu()
{
	CreateMenu(CLRP, clonk, this, C4MN_Extra_Value, "RGB");
	
	// brighter color row
	AddChannelItem("$IncR$", "ChangeRed",   +1, 0, 0xff0000, r < value_range);
	AddChannelItem("$IncG$", "ChangeGreen", +1, 1, 0x00ff00, g < value_range);
	AddChannelItem("$IncB$", "ChangeBlue",  +1, 2, 0x0000ff, b < value_range);
	if (has_alpha)
	{
		AddChannelItem("$DecA$", "ChangeAlpha", -1, 3, 0x3f7f7f7f, a < value_range);
	}
	else
	{
		AddMenuSpacers(1);
	}
	// player color
	AddMenuItem("$PlrClr$", Format("ToSpecificColor(%d, 4)", GetPlrColorDw(GetOwner(clonk))), CLRP, clonk, CLRP_NoCount, 0, "", C4MN_Add_ImgColor, GetPlrColorDw(GetOwner(clonk)));
	
	// darker color row
	AddChannelItem("$DecR$", "ChangeRed",   -1, 5, 0x7f0000, r > 0);
	AddChannelItem("$DecG$", "ChangeGreen", -1, 6, 0x007f00, g > 0);
	AddChannelItem("$DecB$", "ChangeBlue",  -1, 7, 0x00007f, b > 0);
	if (has_alpha)
	{
		AddChannelItem("$IncA$", "ChangeAlpha", +1, 8, 0xcf7f7f7f, a > 0);
	}
	else
	{
		AddMenuSpacers(1);
	}
	
	// color preview
	AddMenuItem("", "", CLRP, clonk, 12345678, 0, "", C4MN_Add_ImgColor, Color());

	
	// numeric preview row
	AddMenuItem("", "", 0, clonk, Max(1, IndexToByte(r)));
	AddMenuItem("", "", 0, clonk, Max(1, IndexToByte(g)));
	AddMenuItem("", "", 0, clonk, Max(1, IndexToByte(b)));
	if (has_alpha)
	{
		AddMenuItem("", "", 0, clonk, Max(1, IndexToByte(a)));
	}
	else
	{
		AddMenuSpacers(1);
	}
	AddMenuItem("$Accept$", "ApplyColor", CXIV, clonk, CLRP_NoCount);	
}

func KeepMenu(int index)
{
	OpenMenu();
	SelectMenuItem(index, clonk);
}

global func OpenColorPicker(object clonk, object target, int start_color, bool has_alpha, int value_range)
{
	if (value_range < 1)
	{
		value_range = CLRP_DefaultValueRange;
	}
	
	var picker = CreateObject(CLRP, 0, 0, GetOwner(clonk));
	LocalN("clonk",       picker) = clonk;
	LocalN("target",      picker) = target;
	LocalN("has_alpha",   picker) = has_alpha;
	LocalN("use_command", picker) = false;
	LocalN("value_range", picker) = value_range;
	picker->ToSpecificColor(start_color);
}

// alternative call for objects that don't have a OnColorPickerReturn() function.
global func OpenColorPickerCmd(object clonk, string command, int start_color, bool has_alpha, int value_range)
{
	if (!command)
	{
		command = "DebugLog(\"chosen color: #%06X\")";
	}
	if (value_range < 1)
	{
		value_range = CLRP_DefaultValueRange;
	}
	
	var picker = CreateObject(CLRP, 0, 0, GetOwner(clonk));
	LocalN("clonk",       picker) = clonk;
	LocalN("command",     picker) = command;
	LocalN("has_alpha",   picker) = has_alpha;
	LocalN("use_command", picker) = true;
	LocalN("value_range", picker) = value_range;
	picker->ToSpecificColor(start_color);
}