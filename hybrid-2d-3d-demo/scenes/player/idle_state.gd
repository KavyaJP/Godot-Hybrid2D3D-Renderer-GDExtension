extends NodeState

@export var player: Player
@export var sprite: Hybrid2D3DAnimatedSprite

func _on_enter() -> void:
	# Update animation immediately when entering state
	_update_animation()

func _on_physics_process(_delta : float) -> void:	
	# Continually check in case direction changes (e.g. via mouse look or external force)
	_update_animation()
	
	# Apply Gravity & Friction (Standard Idle Physics)
	if not player.is_on_floor():
		player.velocity.y -= WorldPhysics.gravity * _delta
		
	player.velocity.x = move_toward(player.velocity.x, 0, WorldPhysics.friction)
	player.velocity.z = move_toward(player.velocity.z, 0, WorldPhysics.friction)
	player.move_and_slide()

func _update_animation() -> void:
	# We use a slight "epsilon" (0.1) to ignore tiny floating point errors
	var dir = player.direction
	
	# Reset flip (default to looking Right/East)
	sprite.flip_h = false
	
	# --- 1. CARDINALS (N, S, E, W) ---
	if dir.z < -0.1 and abs(dir.x) < 0.1:     # North (-Z)
		sprite.play("idle_n")
	elif dir.z > 0.1 and abs(dir.x) < 0.1:    # South (+Z)
		sprite.play("idle_s")
	elif dir.x > 0.1 and abs(dir.z) < 0.1:    # East (+X)
		sprite.play("idle_e")
	elif dir.x < -0.1 and abs(dir.z) < 0.1:   # West (-X)
		sprite.play("idle_e") # Re-use East...
		sprite.flip_h = true  # ...and flip!

	# --- 2. DIAGONALS (NE, SE, SW, NW) ---
	elif dir.z < -0.1 and dir.x > 0.1:        # South-East
		sprite.play("idle_se")
	elif dir.z > 0.1 and dir.x > 0.1:         # North-East
		sprite.play("idle_ne")
	elif dir.z > 0.1 and dir.x < -0.1:        # North-West
		sprite.play("idle_ne") # Re-use NE...
		sprite.flip_h = true   # ...and flip!
	elif dir.z < -0.1 and dir.x < -0.1:       # South-West
		sprite.play("idle_se") # Re-use SE...
		sprite.flip_h = true   # ...and flip!
		
	# Fallback (Default to Front)
	else:
		sprite.play("idle_s")

func _on_next_transitions() -> void:
	if GameInputEvent.is_jump_just_pressed():
		transition.emit("jump")
		return
	
	if not player.is_on_floor():
		transition.emit("fall")
		return
	
	if GameInputEvent.is_movement_input():
		transition.emit("walk")

func _on_exit() -> void:
	sprite.stop()
