#ifndef MAGNET_H
#define MAGNET_H

#include "body.h"
#include "base.h"
#include "broadphase.h"

class MagnetPairing;

class Magnet{
	friend class MagnetPairing;
	friend class MagnetSolver;
	public:
		Magnet(Body* b, Vector2D pos, double s, bool p)
			: body(b), position(pos), strength(s), polarity(p) {}
	private:
		Body* body;
		Vector2D position;
		double strength;
		bool polarity;
};

class MagnetPairing{
	public:
		MagnetPairing(Magnet* a, Magnet* b) : A(a), B(b) {lastLambda = 5000; lastPC = 2;}
		void ApplyImpulse();
		void PositionalCorrection();
	private:
		double lastLambda;
		double lastPC;
		Magnet* A;
		Magnet* B;
};

class MagnetSolver{
	public:
		void Initialize();
		void SolveVelocities();
		void SolvePositions();
		void AddMagnet(Magnet* m);
	private:
		//BroadPhase bp;
		std::vector<Magnet*> magnet;
		std::vector<MagnetPairing> pair;
};

#endif