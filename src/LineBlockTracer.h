
// LineBlockTracer.h

// Declares the cLineBlockTracer class representing a cBlockTracer that traces along a straight line between two points





#pragma once

#include "BlockTracer.h"





// fwd: Chunk.h
class cChunk;

// fwd: cChunkMap.h
typedef cItemCallback<cChunk> cChunkCallback;






class cLineBlockTracer :
	public cBlockTracer,
	public cChunkCallback
{
	typedef cBlockTracer super;

public:
	enum eLineOfSight
	{
		// Bit flags used for LineOfSightTrace's Sight parameter:
		losAir = 1,    // Can see through air
		losWater = 2,  // Can see through water
		losLava = 4,   // Can see through lava

		// Common combinations:
		losAirWaterLava = losAir | losWater | losLava,
		losAirWater = losAir | losWater,
	};


	cLineBlockTracer(cWorld & a_World, cCallbacks & a_Callbacks);

	/** Traces one line between Start and End; returns true if the entire line was traced (until OnNoMoreHits()) */
	bool Trace(double a_StartX, double a_StartY, double a_StartZ, double a_EndX, double a_EndY, double a_EndZ);

	// Utility functions for simple one-line usage:

	/** Traces one line between Start and End; returns true if the entire line was traced (until OnNoMoreHits()) */
	static bool Trace(cWorld & a_World, cCallbacks & a_Callbacks, double a_StartX, double a_StartY, double a_StartZ, double a_EndX, double a_EndY, double a_EndZ);

	/** Traces one line between Start and End; returns true if the entire line was traced (until OnNoMoreHits()) */
	static bool Trace(cWorld & a_World, cCallbacks & a_Callbacks, const Vector3d & a_Start, const Vector3d & a_End);

	/** Returns true if the two positions are within line of sight (not obscured by blocks).
	a_Sight specifies which blocks are considered transparent for the trace, is an OR-combination of eLineOfSight constants. */
	static bool LineOfSightTrace(cWorld & a_World, const Vector3d & a_Start, const Vector3d & a_End, int a_Sight);

	/** Traces until the first solid block is hit (or until end, whichever comes first.
	If a solid block was hit, returns true and fills a_HitCoords, a_HitBlockCoords and a_HitBlockFace.
	If a_End is encountered without hitting any solid block, returns false and doesn't touch a_HitCoords, a_HitBlockCoords nor a_HitBlockFace.
	a_HitCoords is the exact coords of the hit,
	a_HitBlockCoords are the coords of the solid block that was hit,
	a_HitBlockFace is the face of the solid block that was hit. */
	static bool FirstSolidHitTrace(
		cWorld & a_World,
		const Vector3d & a_Start, const Vector3d & a_End,
		Vector3d & a_HitCoords,
		Vector3i & a_HitBlockCoords,
		eBlockFace & a_HitBlockFace
	);

protected:
	/** The start point of the trace */
	double m_StartX, m_StartY, m_StartZ;

	/** The end point of the trace */
	double m_EndX, m_EndY, m_EndZ;

	/** The difference in coords, End - Start */
	double m_DiffX, m_DiffY, m_DiffZ;

	/** The increment at which the block coords are going from Start to End; either +1 or -1 */
	int m_DirX, m_DirY, m_DirZ;

	/** The current block */
	int m_CurrentX, m_CurrentY, m_CurrentZ;

	/** The face through which the current block has been entered */
	eBlockFace m_CurrentFace;


	/** Adjusts the start point above the world to just at the world's top */
	void FixStartAboveWorld(void);

	/** Adjusts the start point below the world to just at the world's bottom */
	void FixStartBelowWorld(void);

	/** Calculates the XZ coords of an intersection with the specified Yconst plane; assumes that such an intersection exists */
	void CalcXZIntersection(double a_Y, double & a_IntersectX, double & a_IntersectZ);

	/** Moves m_Current to the next block on the line; returns false if no move is possible (reached the end) */
	bool MoveToNextBlock(void);

	// cChunkCallback overrides:
	virtual bool Item(cChunk * a_Chunk) override;
} ;





