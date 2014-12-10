/* implementations of code algorithms
   --from eric's last year's osu final project lessons:
   ----> vector graphics: spellcards implement y=x^3 movement
   ------>bullets implement different functions
   ---------1)sin/cos
   ---------2)homing-tracking
   ---------3)bezier curves?
   ---------4)circular paths deviating from a point
   ------------>(incrementing normal and angular acceleration

   ---->data structures: lookup tables again ( for bezier )
   ---->

   --from Barak's experience:
   ----> Screw up memory/object management, screw up life
   ------> Make everything flexible
   ------> Use states (game state, state of vars, etc.)
   ----> Fix timer before anything else; check for significant lag and stability
   ------> if lag significantly, abandon/fix/comment out immediately/think of workaround
   ----------> DO NOT USE ELLIPSE EQUATIONS TO CALCULATE UNIT COLLISION
   ----> Timed commands for bullets
   ----> Fix keyboard input
*/
//all objs that can be drawn in the GUI
var obj={
    hp:depends,
    speed:depends,
    x:depends,
    y:depends,
    var character={
	hp:1,//seriously this is a hit and die game
	lives:3,
	bombs:3
    }
    var mob = {
	//regular monster
	is_boss:bool;
	norm_attack(){},
	var list_of_skills = {
	    spell_1(){},
	    spell_n(){}
	}
    }
    var projectile = {
	
    }
    var bullet = {
    }
}
