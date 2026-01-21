#strict 2
#appendto WOOD
#appendto COAL
#appendto OBRL

func FurnaceFuelValue() {
	if(GetID() == WOOD){
		if(this)
			return GetCon(this)/20;
		else
			return 5;
	}

	if(GetID() == COAL){
		if(this)
			return GetCon(this)/10;
		else
			return 10;
	}

	if(GetID() == OBRL){
		if(this)
			return LocalN("iFillLevel")/10;
		else  //"theoretische" Ölfässer - sprich, aufgerufen per Definition - gelten als voll
			return 20;
	}
}

func BurnUp(pConsumer) {
	if(GetID() != OBRL) return 0;
 ChangeDef(BARL,this);
 return 1; //nicht verschwinden
}