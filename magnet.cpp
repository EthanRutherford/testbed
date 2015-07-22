#include "magnet.h"

void MagnetPairing::ApplyImpulse()
{
	double mA = A->body->planet ? 0 : A->body->mass.iM;
	double mB = B->body->planet ? 0 : B->body->mass.iM;
	double iA = A->body->planet ? 0 : A->body->mass.iI;
	double iB = B->body->planet ? 0 : B->body->mass.iI;
	//mix polarity
	bool attract = A->polarity != B->polarity;
	//mix strength
	double strength = A->strength * B->strength;
	//add polarity to strength
	strength *= attract ? -1 : 1;
	//get relative and absolute positions
	Vector2D rA = A->body->transform * A->position;
	Vector2D rB = B->body->transform * B->position;
	Vector2D cA = rA + A->body->position;
	Vector2D cB = rB + B->body->position;
	Vector2D diff = cB - cA;
	//determine force to apply
	double lambda = strength / diff.LSqr();
	lambda = Clamp(lambda, -std::abs(lastLambda)/2, std::abs(lastLambda)/2);
	lastLambda = lambda;
	normalize(diff);
	Vector2D impulse = diff * lambda;
	A->body->velocity -= impulse * mA;
	A->body->angVel -= iA * cross(rA, impulse);
	B->body->velocity += impulse * mB;
	B->body->angVel += iB * cross(rB, impulse);
}

void MagnetPairing::PositionalCorrection()
{
	double mA = A->body->planet ? 0 : A->body->mass.iM;
	double mB = B->body->planet ? 0 : B->body->mass.iM;
	double iA = A->body->planet ? 0 : A->body->mass.iI;
	double iB = B->body->planet ? 0 : B->body->mass.iI;
	//mix polarity
	bool attract = A->polarity != B->polarity;
	//mix strength
	double strength = A->strength * B->strength;
	//add polarity to strength
	strength *= attract ? -1 : 1;
	//get relative and absolute positions
	Vector2D rA = A->body->transform * A->position;
	Vector2D rB = B->body->transform * B->position;
	Vector2D cA = rA + A->body->position;
	Vector2D cB = rB + B->body->position;
	Vector2D diff = cB - cA;
	//determine force to apply
	double lambda = strength / diff.LSqr();
	lambda = Clamp(lambda, -std::abs(lastPC)/2, std::abs(lastPC)/2);
	lastPC = lambda;
	normalize(diff);
	Vector2D impulse = diff * lambda;
	A->body->position -= impulse * mA;
	A->body->orient -= iA * cross(rA, impulse);
	B->body->position += impulse* mB;
	B->body->orient += iB * cross(rB, impulse);
}

void MagnetSolver::Initialize()
{
	pair.clear();
	for(int i = 0; i < magnet.size(); i++)
	{
		for (int j = i+1; j < magnet.size(); j++)
		{
			//skip magnets on same body
			if (magnet[i]->body == magnet[j]->body)
				continue;
			pair.emplace_back(magnet[i], magnet[j]);
		}
	}
}

void MagnetSolver::SolveVelocities()
{
	for (int i = 0; i < pair.size(); i++)
		pair[i].ApplyImpulse();
}

void MagnetSolver::SolvePositions()
{
	for (int i = 0; i < pair.size(); i++)
		pair[i].PositionalCorrection();
}

void MagnetSolver::AddMagnet(Magnet* m)
{
	magnet.emplace_back(m);
}

