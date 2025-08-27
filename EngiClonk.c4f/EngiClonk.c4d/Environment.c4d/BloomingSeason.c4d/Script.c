/*-- Vogelgezwitscher --*/

#strict
local done;

func Initialize(){
	if(!done)
	for(var grs in FindObjects(Find_ID(GRAS))){
		SetColorDw(RandomRGBa(true), grs);
		if (!Random(3)) SetPhase(2, grs);
		if (!Random(3)) SetPhase(3, grs);
	};
	
	done = true;
}