#include "ColorDefines.h"

std::string ColorFormatToString(const ColorFormat& format)
{
	switch (format)
	{
	case (DEFAULT): return ("DEFAULT");
		break;
	case (RGBA): return ("RGBA");
		break;
	case (ARGB): return ("ARGB");
		break;
	case (ABGR): return ("ABGR");
		break;
	case (BGRA): return ("BGRA");
		break;
	case (RGB): return ("RGB");
		break;
	default: return ("UNKONWN");
		break;
	}

}

ColorFormat ColorFormatFromString(const std::string& format)
{
	if (format == "RGB") {
		return (RGB);
	}
	else if (format == "RGBA") {
		return (RGBA);
	}
	else if (format == "ARGB") {
		return (ARGB);
	}
	else if (format == "BGRA") {
		return (BGRA);
	}
	else if (format == "ABGR") {
		return (ABGR);
	}
	else {
		return (DEFAULT);
	}
}

std::string ColorNameToString(const ColorName& colorName)
{
	switch (colorName)
	{
	case(WHITE): return ("WHITE");
		break;
	case(BLACK): return ("BLACK");
		break;
	case(GREY): return ("GREY");
		break;
	case(RED): return ("RED");
		break;
	case(BLUE): return ("BLUE");
		break;
	case(GREEN): return ("GREEN");
		break;
	case(ORANGE): return ("ORANGE");
		break;
	case(PURPLE): return ("PURPLE");
		break;
	case(YELLOW): return ("YELLOW");
		break;
	default: return ("UNKONWN");
		break;
	}
}
