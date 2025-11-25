/*-- Neues Szenario --*/

#strict 2

func Initialize() {
	SpawnPlatform();
	//forge setup
  RemoveObject(FindObject2(Find_ID(FALW)));
  CreateObject(FALW);
/*   FindObject2(Find_ID(FALW))->AddForgeable(METL, RGBa(177, 171, 170, 0), 3, 34, 2, "LavaLQM");
  FindObject2(Find_ID(FALW))->AddForgeable(CPIG, RGBa(170, 79, 66, 0), 4, 30, 3, "LavaLQM Lightning");
  FindObject2(Find_ID(FALW))->AddForgeable(TTIG, RGBa(120, 130, 139, 0), 2, 37, 0, "");
  FindObject2(Find_ID(FALW))->AddForgeable(ROCK, RGBa(190, 194, 192, 0), 6, 20, -1, "Fragile");
  FindObject2(Find_ID(FALW))->AddForgeable(GOLD, RGBa(255, 178, 18, 0), 6, 20, -1, "Fragile Glow");
  FindObject2(Find_ID(FALW))->AddForgeable(WOOD, RGBa(202, 140, 50, 0), 1, 45, -5, "Fragile Windy Burnable");
  FindObject2(Find_ID(FALW))->AddForgeable(COAL, RGBa(55, 53, 54, 0), 5, 25, 2, "Fragile Burnable Fuming");
  FindObject2(Find_ID(FALW))->AddForgeable(FLNT, RGBa(255, 0, 0, 0), 5, 26, 5, "Explode");
  FindObject2(Find_ID(FALW))->AddForgeable(HONY, RGBa(156, 57, 13, 1), 0, 36, -6, "Sticky Burnable WaterLQM Skew");
  FindObject2(Find_ID(FALW))->AddForgeable(CRYS, RGBa(141, 169, 242, 127), 0, 45, 7, "Regen Glow Fragile");
  FindObject2(Find_ID(FALW))->AddForgeable(MGNT, RGBa(0, 100, 255, 0), 3, 34, 2, "LavaLQM Attractive Magnet"); */
  //all strucutres become player 1's.
  for(var struct in FindObjects(Find_Category(C4D_Structure))){
	  SetOwner(0,struct);
  }
  
  //Liquid filled tanks, endless'ish liquid :D
    var Cabin = FindObject(HUT3);
	for(var cont in FindObjects(Find_Container(Cabin))){
		RemoveObject(cont);
	}
  var Tanks = [];
  Tanks[0] = CreateForgedObjectCustom(ALQT,0,0,0,RandomRGBa(),1,99,99,"");
  Tanks[0]->InsertLiquidPx("DuroLava",Tanks[0]->MaxAmount());
  SetName("ENDLESS LAVA TANK",Tanks[0]);
  
  Tanks[1] = CreateForgedObjectCustom(ALQT,0,0,0,RandomRGBa(),1,99,99,"");
  Tanks[1]->InsertLiquidPx("Water",Tanks[1]->MaxAmount());
  SetName("ENDLESS WATER TANK",Tanks[1]);
  
  Tanks[2] = CreateForgedObjectCustom(ALQT,0,0,0,RandomRGBa(),1,99,99,"");
  Tanks[2]->InsertLiquidPx("Acid",Tanks[2]->MaxAmount());
  SetName("ENDLESS ACID TANK",Tanks[2]);
  
  Tanks[3] = CreateForgedObjectCustom(ALQT,0,0,0,RandomRGBa(),1,99,99,"");
  Tanks[3]->InsertLiquidPx("Oil",Tanks[3]->MaxAmount());
  SetName("ENDLESS OIL TANK",Tanks[3]);
  
  Tanks[4] = CreateObject(_CPL);
  LocalN("plan_id",Tanks[4]) = KDNT;
  
  Tanks[5] = CreateObject(_CPL);
  LocalN("plan_id",Tanks[5]) = PGTP;
  
  for(var tank in Tanks){
	  Enter(Cabin,tank);
  }
  
  FindObject(QUAR)->SetOrder(GOLD);
  
  var Tele = CreateConstruction(PGTP,(LandscapeWidth()/2)-(LandscapeWidth()/4),LandscapeHeight()/2,0,100,false,false);
	SetName("Ground - Left Side",Tele);
  while(VerticesStuck(Tele)){
	  SetY(GetY(Tele)-1,Tele);
	 }
  Tele = CreateConstruction(PGTP,(LandscapeWidth()/2)+(LandscapeWidth()/4),LandscapeHeight()/2,0,100,false,false);
	SetName("Ground - Right Side",Tele);
  while(VerticesStuck(Tele)){
	  SetY(GetY(Tele)-1,Tele);
	 }
	
  MakeCavern();
  
    var Tele = CreateConstruction(PGTP,(LandscapeWidth()/2)-(LandscapeWidth()/4),LandscapeHeight(),0,100,false,false);
	SetName("Cavern - Left Side",Tele);
  while(VerticesStuck(Tele)){
	  SetY(GetY(Tele)-1,Tele);
	 }
  Tele = CreateConstruction(PGTP,(LandscapeWidth()/2)+(LandscapeWidth()/4),LandscapeHeight(),0,100,false,false);
	SetName("Cavern - Right Side",Tele);
  while(VerticesStuck(Tele)){
	  SetY(GetY(Tele)-1,Tele);
	 }
}

func InitializePlayer(int iPlr){
	SetPosition(GetX(FindObject(HUT3)), GetY(FindObject(HUT3)), GetCrew(iPlr,0));
	var flag = CreateObject(FLAG, 130+(5*iPlr),509, 0);
	Enter(FindObject(HUT3),flag);
	SetOwner(iPlr, flag);
	SetFoW (0, iPlr);
	var i, id; while (id = GetDefinition(i++)){
		if((GetCategory(,id) & C4D_Structure) && !(GetCategory(,id) & C4D_Knowledge)) continue;
		if((GetCategory(,id) & C4D_Vehicle) && !(GetCategory(,id) & C4D_Knowledge)) continue;
		SetPlrKnowledge(iPlr, id);
		var ok = false;
		var def = id;
		if(GetCategory(,def) & 512) ok = true;
		if(GetCategory(,def) & 1024) ok = true;
		if(GetCategory(,def) & 2048) ok = true;
		if(GetCategory(,def) & 16384) ok = true;
		if(GetDefCoreVal("CrewMember", "DefCore", def)) ok = true;
		if(GetCategory(,def) & C4D_Structure) ok = false;
		if(GetComponent(DUMM,,,def) > 0) ok = false;
		if(!ok) continue;
		DoHomebaseMaterial(iPlr,id,999);
	}
}

public func RelaunchPlayer(Owner){
	var conk = CreateObject(CLNK,LandscapeWidth()/2,LandscapeHeight()/3);
	var Cabin = FindObject(HUT3);
	if(Cabin) Enter(Cabin,conk);
	
	SetOwner(Owner,conk);
	MakeCrewMember(conk,Owner);
	SetCursor(Owner,conk);
}

//generation test
global func SpawnPlatform(){
	var x = LandscapeWidth() / 2;
	var y = 200;
	var height = 15; //50 pixels high,  ok to walk on
	var width = 0;
	var offset = 5;
	var OutsideOffset = 10;
	var Banned = [BAS7,CTW0,CST1,CST2,CST3,HUT1,HUT2,DRCK,ELEV,FNDR,GIDL,IDOL,IGLO,POWR,PUMP,RSRC,SAWM,SGNL,TWR2,BMBX,CT5P,AHUT,ACLD,MWKS,WZKP];
	
	x-=OutsideOffset;
	var o,i;
	while(o = GetDefinition(i++,C4D_Structure)){
		if(InArray(o,Banned) != -1) continue;
		if(!(GetCategory(,o) & C4D_Knowledge)) continue;
		width += GetWidth(o)+offset;
		width += offset;
	}
	width+=OutsideOffset*2;
	x -= width/2;
	var iX = x+10;
	i = 0;
	while(o = GetDefinition(i++,C4D_Structure)){
		if(InArray(o,Banned) != -1) continue;
		if(!(GetCategory(,o) & C4D_Knowledge)) continue;
		var pX = iX;
		iX+=(GetWidth(o)+offset);
		pX -= iX;
		var build = CreateConstruction(o,iX+pX/2,y,0,100,false,false);
		build->SetOwner(0);
		iX += offset;
	}
	
	DrawMaterialQuad("Brick",x,y,x+width,y,x+width,y+height,x,y+height,true);
	CreateConstruction(ELEV,x-14,y,0,100,false,false);
	CreateConstruction(ELEV,x+width+15,y,0,100,false,false);
	
	var Tele = CreateConstruction(PGTP,x+75+offset,y,0,100,false,false);
	SetName("Base",Tele);
}

global func GetWidth(cid){
	if(DefinitionCall(cid,"BasementRealWidth")) return(DefinitionCall(cid,"BasementRealWidth"));
	else if(DefinitionCall(cid,"BasementWidth")) return(DefinitionCall(cid,"BasementWidth"));
	else{
		return(GetDefCoreVal("Width","DefCore",cid));
	}
}

global func MakeCavern(){
	var TopY = LandscapeHeight() - 600;
	var BotY = LandscapeHeight() - 50;
	
	DrawMaterialQuad("Tunnel",0,TopY,LandscapeWidth(),TopY,LandscapeWidth(),BotY,0,BotY);
}