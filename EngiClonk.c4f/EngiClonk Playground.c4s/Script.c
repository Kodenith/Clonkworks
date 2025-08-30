//Script for the scenario, not much here.
//the admin door has interesting stuff, go see that :)

#strict


func Initialize() {
   CreateObject(ECAC, 374, 1075);
   CreateObject(ADSG, 374, 1075-15);
   var torch;
   torch = CreateObject(TOR2, 374-15, 1090);
   torch->SetAction("Burning");
   torch = CreateObject(TOR2, 374+15, 1090);
   torch->SetAction("Burning");
   torch->SetDir(1);
   
   //Forge Setup
  FindObject2(Find_ID(FALW))->AddForgeable(METL, RGBa(177, 171, 170, 0), 3, 34, 2, "Meltable HTM");
  FindObject2(Find_ID(FALW))->AddForgeable(CPIG, RGBa(170, 79, 66, 0), 4, 30, 3, "Meltable Lightning");
  FindObject2(Find_ID(FALW))->AddForgeable(TTIG, RGBa(120, 130, 139, 0), 2, 37, 0, "");
  FindObject2(Find_ID(FALW))->AddForgeable(ROCK, RGBa(190, 194, 192, 0), 6, 20, -1, "Fragile");
  FindObject2(Find_ID(FALW))->AddForgeable(GOLD, RGBa(255, 178, 18, 0), 6, 20, -1, "Fragile Glow");
  FindObject2(Find_ID(FALW))->AddForgeable(WOOD, RGBa(202, 140, 50, 0), 1, 45, -5, "Fragile Windy Burnable");
  FindObject2(Find_ID(FALW))->AddForgeable(COAL, RGBa(55, 53, 54, 0), 5, 25, 2, "Fragile Burnable Fuming");
  FindObject2(Find_ID(FALW))->AddForgeable(FLNT, RGBa(255, 0, 0, 0), 5, 26, 5, "Explode");
  FindObject2(Find_ID(FALW))->AddForgeable(HONY, RGBa(156, 57, 13, 1), 0, 36, -6, "Sticky Burnable WaterLQM Skew");
  FindObject2(Find_ID(FALW))->AddForgeable(CRYS, RGBa(141, 169, 242, 127), 0, 45, 7, "Regen Glow Fragile");
  return(1);
}

func InitializePlayer(int iPlr){
	SetPosition(102, 589, GetCrew(iPlr,0));
	var flag = CreateObject(FLAG, 130+(5*iPlr),509, 0);
	SetOwner(iPlr, flag);
	SetFoW (0, iPlr);
	var i, id; while (id = GetDefinition(i++)) SetPlrKnowledge(iPlr, id);
	if(MAGE){
		DoHomebaseMaterial(iPlr, MAGE, 3);
	}
}
