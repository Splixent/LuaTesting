-- /scripts/main.luau
-- Load entities directly instead of using require
local entities = {
	list = {
		{
			x = 200,
			y = 200,
			vx = 40,
			vy = 0,
			radius = 30,
		},
		{
			x = 400,
			y = 300,
			vx = -20,
			vy = 15,
			radius = 20,
		},
       {
			x = 500,
			y = 100,
			vx = 40,
			vy = 0,
			radius = 30,
		},
		{
			x = 300,
			y = 100,
			vx = -20,
			vy = 15,
			radius = 20,
		},
	}
}

-- Graphics functions (will be set in init)
local gfx

local time = 0

function init()
	print("Luau init() called from C++")
	-- Get the gfx table that was registered by C++
	gfx = _G.gfx
end

function update(deltaTime)
	time += deltaTime

	gfx.beginDrawing()
	
	-- simple background color that pulses with time
	local brightness = 0.2 + 0.2 * math.sin(time * 2)
	gfx.clearBackground(brightness, brightness, brightness)

	-- update and draw entities
	for _, entity in ipairs(entities.list) do
		entity.x += entity.vx * deltaTime
		entity.y += entity.vy * deltaTime

		gfx.drawCircle(entity.x, entity.y, entity.radius)
	end

	gfx.endDrawing()
end