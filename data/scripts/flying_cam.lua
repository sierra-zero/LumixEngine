-- add to entity with a camera component
-- behaves similar to camera in scene view

local yaw = 0
local pitch = 0
local forward = 0
local dyaw = 0
local dpitch = 0
local rmb_down = 0

function addVec3(a, b)
    return {a[1] + b[1], a[2] + b[2], a[3] + b[3]}
end

function mulVec3(a, f)
    return {a[1] * f, a[2] * f, a[3] * f}
end

function mulQuat(a, b)
    return { 
        a[4] * b[1] + b[4] * a[1] + a[2] * b[3] - b[2] * a[3],
        a[4] * b[2] + b[4] * a[2] + a[3] * b[1] - b[3] * a[1],
        a[4] * b[3] + b[4] * a[3] + a[1] * b[2] - b[1] * a[2],
        a[4] * b[4] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3]
    }
end

function update(dt)
    yaw = yaw + dyaw * dt
    pitch = pitch + dpitch * dt
    dyaw = 0
    dpitch = 0

    if pitch > 1 then pitch = 1 end
    if pitch < -1 then pitch = -1 end

    local dir = { math.sin(yaw), 0, math.cos(yaw) }
    local pos = addVec3(this.position, mulVec3(dir, -forward * dt))
    this.position = pos
    local yaw_quat = { 0, math.sin(yaw * 0.5), 0, math.cos(yaw * 0.5) }
    local pitch_quat = { math.sin(pitch * 0.5), 0, 0, math.cos(pitch * 0.5) }
    this.rotation = mulQuat(yaw_quat, pitch_quat)
end

function onInputEvent(event)
	if event.type == LumixAPI.INPUT_EVENT_BUTTON then
        if event.device.type == LumixAPI.INPUT_DEVICE_MOUSE and event.key_id > 0 then
            rmb_down = event.down
            Gui.enableCursor(not event.down)

        elseif event.device.type == LumixAPI.INPUT_DEVICE_KEYBOARD then
			if event.key_id == string.byte("W") then
                if event.down then
                    forward = 1
                else 
                    forward = 0
                end
			end
			if event.key_id == string.byte("S") then
                if event.down then
                    forward = -1
                else 
                    forward = 0
                end
			end
		end		
	elseif event.type == LumixAPI.INPUT_EVENT_AXIS then
		if event.device.type == LumixAPI.INPUT_DEVICE_MOUSE and rmb_down then
			dyaw = dyaw + event.x * -0.1;
			dpitch = dpitch + event.y * -0.1;
		end
	end
end

function start()
end