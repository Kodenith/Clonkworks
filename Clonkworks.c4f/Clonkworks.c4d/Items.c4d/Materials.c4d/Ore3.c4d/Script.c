/*--- Eisenerz ---*/

#strict

func Hit () {
  Sound("RockHit*");
}

/* F�r Erzmine */



func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(200); }
func SmeltResult(){ return(TTIG);} 
func IsMetal(){ return(true); }