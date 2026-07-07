/*-- Tera-Flint --*/

#strict

#include FLNT

public func ExplodeSize() { return(RandomX(5,10)); }

func Trail(){
    CreateParticle("Fire",0,0,RandomX(-2,2),RandomX(-2,2),RandomX(20,45),RGBa(255,255,255));
    var windMove = GetWind(0,0)/30;
    if(windMove != 0) SetXDir(GetXDir()+windMove);

    if(GetXDir() == 0 && GetYDir() == 0) Explode(ExplodeSize());
}