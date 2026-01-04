/*-- Vogelgezwitscher --*/

#strict

local Chance;

func Initialize(){
	SetPosition(0,0);
	Chance = 25;
	if( FindObject2(Find_ID(GetID()), Find_Exclude(this())) ){
		LocalN("Chance",FindObject2(Find_ID(GetID()), Find_Exclude(this()))) -= 1;
		RemoveObject();
	}
}

public func TryMeteo(){
	Chance = BoundBy(Chance,0,25);
	if(Random(Chance) <= 0){
		CreateObject(GiantMeteoriteType(),Random(LandscapeWidth()),0)->LaunchMete();
	}
}

global func GiantMeteoriteType(){ return(RDEP); }