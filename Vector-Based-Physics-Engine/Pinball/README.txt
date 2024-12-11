Wrecker's Mayhem

Here you are in a construction site, seeing people build things then 
get destroyed in a cycle

controls
wasd to control the player
space to detach your wrecking ball
	recalls your wrecking ball if detached

left click to build pillars
	cost materials
right click to build walls
	cost materials

how to play
control wrecker to spin around the wrecking ball
1 ball will spawn in the middle on the map, moving in a random direction
in the neutral zone, there will be different zones spawning as well as obstacles
zones such as ditches will push the ball in different directions
pillars and walls will be destroyed if your wrecking ball is fast enough
slowing down the wrecking ball before it gets recalled
recalling your wrecking ball destroys everything in its path as well
neutral balls will simple destroy the obstacle while bouncing back, 
changing their alignment if the obstacle belongs to either player or enemy

hitting neutral balls will turn them into your "allies", collecting powerups for you
such as materials which can be used to build walls
the walls work the same way, except that your own projectiles dont destroy them 
and simply past through, however they will damage the enemies walls
the same occurs with the enemies' projectile. 

things added after presentation
	add materials -> building items
		fixed building limits
	Powerups
		health +3 health to specific faction
		materials +50 materials
		split balls, +1 ball of same color

	refined AI, smoother movement

	home base for each side, collision, reduce hp

	particles
		collision particles
		speed sparks

	flipper at edge, plan to add more
		changed collision code for angular velocity
		fixed bug for pivot collision
			pending bugs: unintended direction collision
	
	thick walls -> changed collision code to make it 1 entity
		made sure the corners have better collision