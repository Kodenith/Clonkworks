//This rule is used by the forge to fetch materials it can use, while also making it easy to use without modifying object code.

#strict

local ForgeableIDs;
local ForgeableColors;
local ForgeableMass;

protected func Initialize()
{
  ForgeableIDs = [];
  ForgeableColors = [];
  ForgeableMass = [];
}
func AddForgeable(id ObjectID, int Color, int Mass){
	//adding a new spot for a new forgeable
	var newspot = GetLength(ForgeableIDs)+1;
	SetLength(ForgeableIDs, GetLength(ForgeableIDs)+1);
	
	SetLength(ForgeableColors, GetLength(ForgeableColors)+1);
	SetLength(ForgeableMass, GetLength(ForgeableMass)+1);
	
	//adding the new values
	ForgeableIDs[newspot] = ObjectID;
	ForgeableColors[newspot] = Color;
	ForgeableMass[newspot] = Mass;
	
	Log("Added (%i, ColorID: %d, Mass: %d) to Forgeables", ForgeableIDs[newspot], ForgeableColors[newspot], ForgeableMass[newspot]);
	
}


protected func Activate(caller) {
  MessageWindow(GetDesc(), caller);
  return(1);
}


