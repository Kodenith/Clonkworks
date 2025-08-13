//This rule is used by the forge to fetch materials it can use, while also making it easy to use without modifying object code.

#strict

local ForgeableIDs;
local ForgeableColors;
local ForgeableMass;
local ForgeableSpeed;
local ForgeablePower;

protected func Initialize()
{
  ForgeableIDs = [];
  ForgeableColors = [];
  ForgeableMass = [];
  ForgeableSpeed = [];
  ForgeablePower = [];
}
func AddForgeable(id ObjectID, int Color, int Mass, int Speed, int Power){
	//adding a new spot for a new forgeable
	var newspot = GetLength(ForgeableIDs)+1;
	SetLength(ForgeableIDs, GetLength(ForgeableIDs)+1);
	SetLength(ForgeableColors, GetLength(ForgeableColors)+1);
	SetLength(ForgeableMass, GetLength(ForgeableMass)+1);
	SetLength(ForgeableSpeed, GetLength(ForgeableSpeed)+1);
	SetLength(ForgeablePower, GetLength(ForgeablePower)+1);
	
	//adding the new values
	ForgeableIDs[newspot] = ObjectID;
	ForgeableColors[newspot] = Color;
	ForgeableMass[newspot] = Mass;
	ForgeableSpeed[newspot] = Speed;
	ForgeablePower[newspot] = Power;
	
	Log("Added (%i, ColorID: %d, Mass: %d, Speed: %d, Power: %d) to Forgeables", ForgeableIDs[newspot], ForgeableColors[newspot], ForgeableMass[newspot], ForgeableSpeed[newspot], ForgeablePower[newspot]);
	
}


protected func Activate(caller) {
  MessageWindow(GetDesc(), caller);
  return(1);
}


