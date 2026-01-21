/*-- Erzabbau --*/

#strict
#include GOAL

local Sold;

public func IsFulfilled()
{
  // Erz noch nicht gezählt?
  var OreCnt = GetMaterialCount(Material("CopperOre"));
  if (OreCnt == -1) return();
  // noch zu viel?
  if (OreCnt >= 150) return();
  // noch Erzstücke abzubauen?
  return(!ObjectCount(ORE2));
}

public func IsFulfilledforPlr (plr)
{
  if (!IsFulfilled()) return ();
  var i = GetPlayerCount();
  while (i--)
    if (Local (GetPlayerByIndex (i)) > Local (plr)) return (0);
  return (1);
}

func SellID () { return (ORE2); }

public func PlayerHasSold (plr, obj)
{
  if (GetID (obj) == SellID ())
  {
    ++Local (plr);
    ++Sold;
  }
}

public func PlayerHasBought (plr, obj)
{
  if (GetID (obj) == SellID ())
  {
    --Local (plr);
    --Sold;
  }
}

protected func Activate(iPlayer)
{
  // Alles abgebaut
  if (IsFulfilled()) 
    return(MessageWindow("$MsgGoalFulfilled$", iPlayer));
  var oreTotal = ObjectCount2(Find_ID(ORE2));
  var oreOutside = ObjectCount2(Find_ID(ORE2), Find_NoContainer());
  // Noch nicht alles abgebaut (fest Menge noch unbekannt)
  if (GetMaterialCount(Material("CopperOre")) == -1)
    return(MessageWindow(Format("$MsgGoalUnfulfilled2$", oreTotal, oreOutside), iPlayer));
  // Noch nicht alles abgebaut ( -50 statt -150 weil beim / 100 abgerundet wird )
  MessageWindow(Format("$MsgGoalUnfulfilled$", Max(GetMaterialCount(Material("CopperOre")) - 50, 0) / 100, oreTotal, oreOutside), iPlayer);
}

public func GetHighestSold(){
  var i = GetPlayerCount();
  var Top = 0;
  while (i--)
    if (Local(GetPlayerByIndex (i)) > Top) Top = Local(GetPlayerByIndex (i));
  return(Top);
}

protected func Activate(iPlayer)
{
  var oreTotal = ObjectCount2(Find_ID(ORE2));
  for(var i in FindObjects(Find_ID(CDEP))){
	  oreTotal -= ContentsCount(ORE2,i);
  }
  var oreOutside = ObjectCount2(Find_ID(ORE2), Find_NoContainer());
  var deposits = ObjectCount(CDEP);
  var Units = Max(GetMaterialCount(Material("CopperOre")) - 50, 0) / 100;
  var Sold = Local(iPlayer);
  var HighestSold = GetHighestSold();
  
  var pClonk = GetCursor(iPlayer);
  
  CreateMenu(GetID(),pClonk,this(),0,GetName(),0,1);
  AddMenuItem(GetDesc(),0,0,pClonk);
  AddMenuItem("$TxtTasklist$",0,0,pClonk);
  if(oreTotal)
	AddMenuItem(Format("$TxtChunks1$",oreTotal,oreOutside),0,_MRK,pClonk);
  else AddMenuItem("$TxtChunks2$",0,_MRK,pClonk,0,0,0,2,1);
  
  if(Units && Units != -1)
	AddMenuItem(Format("$TxtOre1$",Units),0,_MRK,pClonk);
  else AddMenuItem("$TxtOre2$",0,_MRK,pClonk,0,0,0,2,1);
  
  if(deposits)
	AddMenuItem(Format("$TxtDepo1$",deposits),0,_MRK,pClonk);
  else AddMenuItem("$TxtDepo2$",0,_MRK,pClonk,0,0,0,2,1);
  
  if(FindObject(RVLR)){
	  if(Sold < HighestSold){
		  AddMenuItem(Format("$TxtComp1$",Sold,HighestSold),0,RVLR,pClonk);
	  }else{
		  AddMenuItem(Format("$TxtComp2$",Sold),0,RVLR,pClonk);
	  }
	  
	  if(IsFulfilledforPlr(iPlayer))  AddMenuItem("$TxtComplete$",0,0,pClonk);
  }else{
	  if(IsFulfilled())  AddMenuItem("$TxtComplete$",0,0,pClonk);
  }
}
