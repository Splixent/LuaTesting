-- /scripts/main.luau
local entities = require("scripts.entities")

-- Graphics functions (will be set in init)

local time = 0

function update(deltaTime)
	time = time + deltaTime

	gfx.beginDrawing()
	
	-- simple background color that pulses with time
	local brightness = 0.2 + 0.2 * math.sin(time * 2)
	gfx.clearBackground(brightness, brightness, brightness)

	-- update and draw entities
	for _, entity in ipairs(entities.list) do
		entity.x = entity.x + entity.vx * deltaTime
		entity.y = entity.y + entity.vy * deltaTime

		gfx.drawCircle(entity.x, entity.y, entity.radius)
	end

	gfx.endDrawing()
end