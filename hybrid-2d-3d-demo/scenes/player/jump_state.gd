extends NodeState

@export var player: Player
@export var sprite: Hybrid2D3DAnimatedSprite

var landing_timer : float = 0.0
var is_landing : bool = false

func _on_enter() -> void:
	is_landing = false
	landing_timer = 0.0
	
	player.velocity.y = player.jump_velocity
	
	# Start with launching frame (1)
	_update_jump_animation(player.direction, 1)

func _on_physics_process(delta: float) -> void:
	# --- PHASE 1: LANDING SEQUENCE ---
	if is_landing:
		player.velocity.x = 0
		player.velocity.z = 0
		
		# Force Landing Frame (4)
		sprite.set_pose(sprite.animation, 4)
		
		landing_timer -= delta
		if landing_timer <= 0:
			if GameInputEvent.is_movement_input():
				transition.emit("walk")
			else:
				transition.emit("idle")
		return

	# --- PHASE 2: AIR PHYSICS ---
	player.velocity.y -= WorldPhysics.gravity * delta
	
	var input_dir = GameInputEvent.get_movement_input()
	if input_dir != Vector3.ZERO:
		player.direction = input_dir
		player.velocity.x = input_dir.x * player.speed * player.air_control
		player.velocity.z = input_dir.z * player.speed * player.air_control
	else:
		player.velocity.x = move_toward(player.velocity.x, 0, player.speed * 0.05)
		player.velocity.z = move_toward(player.velocity.z, 0, player.speed * 0.05)

	# --- PHASE 3: ANIMATION LOGIC ---
	
	# Determine which frame we need based on vertical speed
	var target_frame = 3 # Default: Falling
	if player.velocity.y > (player.jump_velocity * 0.5):
		target_frame = 1 # Fast Rising (Launch)
	elif player.velocity.y > 0:
		target_frame = 2 # Slow Rising (Apex)
	
	# Apply direction AND frame in one go
	_update_jump_animation(player.direction, target_frame)

	player.move_and_slide()

func _on_next_transitions() -> void:
	if player.is_on_floor() and not is_landing:
		is_landing = true
		landing_timer = 0.15

# Modified to accept the target frame
func _update_jump_animation(dir: Vector3, frame_index: int) -> void:
	sprite.flip_h = false
	var anim_name = "jump_s"
	
	if dir.z < -0.1 and abs(dir.x) < 0.1:     anim_name = "jump_n"
	elif dir.z > 0.1 and abs(dir.x) < 0.1:    anim_name = "jump_s"
	elif dir.x > 0.1 and abs(dir.z) < 0.1:    anim_name = "jump_e"
	elif dir.x < -0.1 and abs(dir.z) < 0.1:   anim_name = "jump_e"; sprite.flip_h = true
	elif dir.z < -0.1 and dir.x > 0.1:        anim_name = "jump_ne"
	elif dir.z > 0.1 and dir.x > 0.1:         anim_name = "jump_se"
	elif dir.z > 0.1 and dir.x < -0.1:        anim_name = "jump_se"; sprite.flip_h = true
	elif dir.z < -0.1 and dir.x < -0.1:       anim_name = "jump_ne"; sprite.flip_h = true

	# CLEAN API: Update everything at once
	sprite.set_pose(anim_name, frame_index)

func _on_exit() -> void:
	sprite.play()
