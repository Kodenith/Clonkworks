/*-- Neues Script --*/

#strict
#appendto CTW2
#appendto CTW6

private func Shoot(id defFoo, object pProjectile, object pShooter)
{
  if(GetID() == CTW6) return(0);
  if (!pProjectile) return(0);
  if (pProjectile->Contained() != GetActionTarget()) return(0);

  var powder = CannonPowderNeeded(pProjectile);
  if(ContentsCount(GUNP, GetActionTarget()) < powder)
  {
    Sound("Error");
    Message("$TxtNotenoughgunpowder1r$", GetActionTarget(), powder);
  }
  else
  {
    for(var i = 0; i < powder; ++ i)
      RemoveObject(FindContents(GUNP, GetActionTarget()) );

    if(CannonSmoke(pProjectile) )
    {
      Smoke(Sin(GetR(), 13), -Cos(GetR(), 13), 20);
      Smoke(Sin(GetR(), 18), -Cos(GetR(), 23), 17);
      Smoke(Sin(GetR(), 22), -Cos(GetR(), 32), 14);
      Smoke(Sin(GetR(), 25), -Cos(GetR(), 40), 11);
    }
    
    // Controller setzen (Killverfolgung)
    var sht = pShooter; if(!sht) sht = GetActionTarget();
    if (GetOCF(pProjectile) & OCF_Alive) pProjectile->SetKiller(GetController(sht));
    pProjectile->SetController(GetController(sht));
    Shoot();

    Exit(pProjectile, Sin(GetR(), 13), -Cos(GetR(), 13), GetR(), Sin(GetR(), CannonPower(pProjectile)), -Cos(GetR(), CannonPower(pProjectile)), 20);
    if(GetOCF(pProjectile) & OCF_CrewMember() ) ObjectSetAction(pProjectile, "Tumble");
    Sound(CannonSound(pProjectile) );
    if(pShooter)
      SetPlrView(pShooter->GetOwner(), pProjectile);
  }
}

public func ComFire(object pClonk,Foothis)
{
  if(GetID() != CTW6) return(_inherited(pClonk));
  Sound("Click");
  SetAction("Attaching", GetActionTarget() );
  SetRDir(0);
    
  // Bis zu 2 Projektile verschie�en
  for(var i = 0; i < 2; ++ i)
  {
    // Projektil suchen
    var c = ContentsCount() - 1, obj, ammo;
    while(obj = Contents(c--) )
    {
      if (obj->~IsArrowPack())
      {
        ammo = obj->~GetItem();
        if (!ammo)
          {
          // Alter Pfeilpaket: Erstmal nur aufteilen; sp�ter wieder versuchen (Rekursionsschutz gegen neues Zusammenpacken)
          Sound("Connect");
          Split2Components(obj);
          break;
          }
      }
      else if(obj->~IsArrow() || GetID(obj) == ADB1)
      {
        ammo = obj;
        break;
      }
    }
  
    // Keins da? :(
    if(!ammo)
    {
      if(!i) Sound("Error");
      else Sound("Click");
      return(0);
    }

    // Sound    
    if(!i) Sound("Arrow");
    
    
    var speed = 15;
    if(GetID(ammo) == ADB1) speed = 22;
    
    // Rotationsgeschwindigkeit
    var rdir = 4;
    var R = GetR();
    if(R < 0) rdir = -4;
    if( Inside(R,-10,10) ) rdir = 0;
    
    // Controller setzen
    if(pClonk)
    ammo->SetController(pClonk->GetController());
    else ammo->SetController(GetActionTarget()->GetController());
    
    // Aussto�
    Exit(ammo, Sin(GetR() - 45 + 90 * i, 9), -Cos(GetR() - 45 + 90 * i, 9), GetR(), Sin(GetR(), speed), -Cos(GetR(), speed), rdir);
    if(pClonk)
      SetPlrView(GetOwner(pClonk), ammo);
    ammo->~Launch();
  }
}

