// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class JYS_API PlayerAbility
{
public:
	PlayerAbility();
	~PlayerAbility();

public:
	float moveSpeed;
	float hp;
	float attackDamage;
	float attackSpeed;
	float rollSpeed;
};
