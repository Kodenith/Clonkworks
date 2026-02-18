/*-- Vogelgezwitscher --*/

#strict

func Initialize(){
	SetPosition(0,0);
}

func Fill(){
	for(var i in FindObjects(Find_Func("IsOreDeposit"))){
		if(ContentsCount(,i) < 100) CreateContents(i->OreDepositType(),i,RandomX(1,10));
	}
}