
// WSSCompact.h

// Interfaces to the cWSSCompact class representing the "Compact" storage schema (PAK-files)





#pragma once
#ifndef WSSCOMPACT_H_INCLUDED
#define WSSCOMPACT_H_INCLUDED

#include "WorldStorage.h"





class cWSSCompact :
	public cWSSchema
{
public:
	cWSSCompact(cWorld * a_World) : cWSSchema(a_World) {}
	virtual ~cWSSCompact();
	
protected:

	struct sChunkHeader;
	typedef std::vector<sChunkHeader *> sChunkHeaders;
	
	/// Implements a cache for a single PAK file; implements lazy-write in order to be able to write multiple chunks fast
	class cPAKFile
	{
	public:
	
		cPAKFile(const AString & a_FileName, int a_LayerX, int a_LayerZ);
		~cPAKFile();

		bool GetChunkData(const cChunkCoords & a_Chunk, int & a_UncompressedSize, AString & a_Data);
		bool SetChunkData(const cChunkCoords & a_Chunk, int a_UncompressedSize, const AString & a_Data);
		bool EraseChunkData(const cChunkCoords & a_Chunk);
		
		bool SaveChunk(const cChunkCoords & a_Chunk, cWorld * a_World);
		bool LoadChunk(const cChunkCoords & a_Chunk, cWorld * a_World);
		
		int GetLayerX(void) const {return m_LayerX; }
		int GetLayerZ(void) const {return m_LayerZ; }
		
	protected:
	
		AString m_FileName;
		int     m_LayerX;
		int     m_LayerZ;
		
		sChunkHeaders m_ChunkHeaders;
		AString       m_DataContents;  // Data contents of the file, cached
		
		int           m_NumDirty;  // Number of chunks that were written into m_DataContents but not into the file
		
		bool LoadChunk(const cChunkCoords & a_Chunk, int a_Offset, sChunkHeader * a_Header, cWorld * a_World);
		bool SaveChunkToData(const cChunkCoords & a_Chunk, cWorld * a_World);  // Saves the chunk to m_DataContents, updates headers and m_NumDirty
		void SynchronizeFile(void);  // Writes m_DataContents along with the headers to file, resets m_NumDirty
	} ;
	
	typedef std::list<cPAKFile *> cPAKFiles;
	
	cCriticalSection m_CS;
	cPAKFiles m_PAKFiles;  // A MRU cache of PAK files
	
	/// Loads the correct PAK file either from cache or from disk, manages the m_PAKFiles cache
	cPAKFile * LoadPAKFile(const cChunkCoords & a_Chunk);
	
	/// Gets chunk data from the correct file; locks CS as needed
	bool GetChunkData(const cChunkCoords & a_Chunk, int & a_UncompressedSize, AString & a_Data);
	
	/// Sets chunk data to the correct file; locks CS as needed
	bool SetChunkData(const cChunkCoords & a_Chunk, int a_UncompressedSize, const AString & a_Data);
	
	/// Erases chunk data from the correct file; locks CS as needed
	bool EraseChunkData(const cChunkCoords & a_Chunk);
	
	/// Loads the chunk from the data (no locking needed)
	bool LoadChunkFromData(const cChunkCoords & a_Chunk, int & a_UncompressedSize, const AString & a_Data, cWorld * a_World);
		
	void LoadEntitiesFromJson(Json::Value & a_Value, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities, cWorld * a_World);

	// cWSSchema overrides:
	virtual bool LoadChunk(const cChunkCoords & a_Chunk) override;
	virtual bool SaveChunk(const cChunkCoords & a_Chunk) override;
	virtual const AString GetName(void) const override {return "compact"; }
} ;





#endif  // WSSCOMPACT_H_INCLUDED




