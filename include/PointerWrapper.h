/**
 * Pointer Wrapper Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file PointerWrapper.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-10-23
 */


#ifndef POINTER_WRAPPER_INCLUDE_H
#define POINTER_WRAPPER_INCLUDE_H
//


#include "InterfaceCloneable.h"

/**
 * Description:
 * @template TTargetType:
 */
template <typename TTargetType>
class CPointerWrapper
{
	/* data: */
public:
private:
	// raw target pointer
	TTargetType* _pTarget;

	/* method: */
public:
	CPointerWrapper(TTargetType* targetPointer);
	CPointerWrapper(const CPointerWrapper<TTargetType>& pointerWrapper);
	~CPointerWrapper();

	TTargetType* getPointer() const;

	/* operator: */
	const CPointerWrapper<TTargetType>& operator =(const CPointerWrapper<TTargetType>& pointerWrapper);
private:
};


//******************************************************

/* Template implementation for CPointerWrapper class: */

/**
 * Description: Ctor.
 */
template <typename TTargetType>
CPointerWrapper<TTargetType>::CPointerWrapper(TTargetType* targetPointer)
{
	_pTarget = targetPointer;
}


/**
 * Description: Copy ctor.
 */
template <typename TTargetType>
CPointerWrapper<TTargetType>::CPointerWrapper(const CPointerWrapper<TTargetType>& pointerWrapper)
{
	const ICloneable& CLONEABLE_TARGET = static_cast<const ICloneable&>(*pointerWrapper.getPointer());
	_pTarget = dynamic_cast<TTargetType*>(CLONEABLE_TARGET.clone());

	//*_pTarget = *pointerWrapper.getPointer();
}


/**
 * Description: Dtor.
 */
template <typename TTargetType>
CPointerWrapper<TTargetType>::~CPointerWrapper()
{
	delete _pTarget;
}


/**
 * Description: Get the raw pointer.
 */
template <typename TTargetType>
TTargetType* CPointerWrapper<TTargetType>::getPointer() const
{
	return _pTarget;
}


template <typename TTargetType>
const CPointerWrapper<TTargetType>& CPointerWrapper<TTargetType>::operator =(const CPointerWrapper<TTargetType>& pointerWrapper)
{
	// If self assignment:
	if (this == &pointerWrapper)
	{
		return *this;
	}

	const ICloneable& CLONEABLE_TARGET = static_cast<const ICloneable&>(*pointerWrapper.getPointer());
	_pTarget = dynamic_cast<TTargetType*>(CLONEABLE_TARGET.clone());
	//*_pTarget = *pointerWrapper.getPointer();

	return *this;
}


//
#endif
