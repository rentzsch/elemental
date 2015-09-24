/****************************************************************************************
	elemental.c

	Copyright (c) 1999-2015 Jonathan 'Wolf' Rentzsch: http://rentzsch.com
	Some rights reserved: http://opensource.org/licenses/mit

	Note that while FindElement() is the only linear-time function here, when
	assertion checking is turned on, many of the so-called constant-time functions
	implictly call FindElement() (some more than once).

	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.
	wolf		Mon, May 3, 1999	Added ___Type macros.
	wolf		Wed, May 31, 2000	Now each element stores a pointer to its list.
									Added GetElementList[Off|Type]() and IsListEmpty().
	wolf		Tue, Feb 27, 2001	CodeWarrior Pro 6 breaks an idiom: implicit
									casting of <type>** to void**. I had to make all
									such casts explicit.

	************************************************************************************/

#include "elemental.h"
#include <assert.h>

#define UseAssertions 1
#include <stdint.h>
#define assertIf( CONDITION, ASSERTION )	if((CONDITION)){assert((ASSERTION));}
#define assertPtr(PTR)						assert((PTR) && (((intptr_t)(PTR))%4)==0)
#define assertPtrIfNotNil(PTR)				if((PTR)){(((intptr_t)(PTR))%4)==0;}

#if	UseAssertions
	void	assertElement( void *element );
	#define	assertElementIfNotNil( ELEMENT )	if((ELEMENT))assertElement((ELEMENT))
	void	assertList( ElementList *list );
#else
	#define	assertElement( ELEMENT )
	#define	assertElementIfNotNil( ELEMENT )
	#define	assertList( LIST )
#endif

	void*
AddOffset(
	void	*element,
	size_t	offset );

	void*
SubtractOffset(
	void	*element,
	size_t	offset );



/****************************************************************************************
*
*	Lifetime
*
****************************************************************************************/
#pragma mark	(Lifetime)

	extern
	void
NewElementList(
	ElementList	*list )
{
	assertPtr( list );
	list->first = list->last = NULL;

}

	extern
	void
DeleteElementList(
	ElementList	*list )
{
	;
}

/****************************************************************************************
*
*	Putters
*
****************************************************************************************/
#pragma mark	(Putters)

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.
	wolf		Wed, May 31, 2000	Updated to support the new list field in Element.

	************************************************************************************/

	void
PutFirstElement(
	void			*element,
	ElementList		*list )
{
	Element	*element_ = (Element*) element;

	assertElement( element );
	assertList( list );
	assert( !FindElement( element, list ) );

	if( list->first ) {
		element_->prev = NULL;
		element_->next = list->first;
		element_->list = list;
		list->first->prev = element_;
		list->first = element_;
	} else {
		list->first = list->last = element_;
		element_->prev = element_->next = NULL;
		element_->list = list;
	}

	assert( list->first == element );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
PutLastElement(
	void			*element,
	ElementList		*list )
{
	Element	*element_ = (Element*) element;

	assertElement( element );
	assertList( list );
	assert( !FindElement( element, list ) );

	if( list->first ) {
		element_->prev = list->last;
		element_->next = NULL;
		element_->list = list;
		list->last->next = element_;
		list->last = element_;
	} else {
		list->first = list->last = element_;
		element_->prev = element_->next = NULL;
		element_->list = list;
	}

	assert( list->last == element );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.
	wolf		Wed, May 31, 2000	Updated to support the new list field in Element.

	************************************************************************************/

	void
PutBeforeElement(
	void			*element,
	void			*before,
	ElementList		*list )
{
	Element	*element_ = (Element*) element;
	Element	*before_ = (Element*) before;

	assertElement( element );
	assertElementIfNotNil( before );
	assert( element != before );
	assertList( list );
	assert( !FindElement( element, list ) );
	assertIf( before, FindElement( before, list ) );

	if( list->first ) {
		if( before_ == NULL || list->first == before_ )
			PutLastElement( element_, list );
		else {
			element_->prev = before_->prev;
			element_->next = before_;
			element_->list = list;
			before_->prev->next = element_;
			before_->prev = element_;
		}
	} else {
		list->first = list->last = element_;
		element_->prev = element_->next = NULL;
		element_->list = list;
	}

	assertIf( before, before_->prev == element && element_->next == before );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.
	wolf		Wed, May 31, 2000	Updated to support the new list field in Element.

	************************************************************************************/

	void
PutAfterElement(
	void			*element,
	void			*after,
	ElementList		*list )
{
	Element	*element_ = (Element*) element;
	Element	*after_ = (Element*) after;

	assertElement( element );
	assertElementIfNotNil( after );
	assert( element != after );
	assertList( list );
	assert( !FindElement( element, list ) );
	assertIf( after, FindElement( after, list ) );

	if( list->first ) {
		if( after_ == NULL || list->last == after_ )
			PutFirstElement( element_, list );
		else {
			element_->prev = after_;
			element_->next = after_->next;
			element_->list = list;
			after_->next->prev = element_;
			after_->next = element_;
		}
	} else {
		list->first = list->last = element_;
		element_->prev = element_->next = NULL;
		element_->list = list;
	}

	assertIf( after, after_->next == element && element_->prev == after );
}

/****************************************************************************************
*
*	Accessors
*
****************************************************************************************/
#pragma mark	-
#pragma mark	(Accessors)

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
FirstElement(
	void			**element,
	ElementList		*list )
{
	assertPtr( element );
	assertList( list );

	*element = list->first;
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
LastElement(
	void			**element,
	ElementList		*list )
{
	assertPtr( element );
	assertList( list );

	*element = list->last;
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
NextElement(
	void		*element,
	void		**nextElement )
{
	Element	*element_ = (Element*) element;

	assertElement( element );
	assertPtr( nextElement );

	*nextElement = element_->next;
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
PrevElement(
	void		*element,
	void		**prevElement )
{
	Element	*element_ = (Element*) element;

	assertElement( element );
	assertPtr( prevElement );

	*prevElement= element_->prev;
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Fri, Apr 16, 1999	Created.

	************************************************************************************/

	bool
FindElement(
	void			*element,
	ElementList		*list )
{
	Element	*element_ = NULL;

	assertElement( element );
	assertList( list );

	FirstElement( (void**) &element_, list );
	while( element_ ) {
		if( element_ == element )
			return( 1 );
		NextElement( element_, (void**) &element_ );
	}
	return( 0 );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Wed, May 31, 2000	Created.

	************************************************************************************/

	ElementList*
GetElementList(
	void	*element )
{
	Element	*element_ = (Element*) element;

	assertElement( element );
	assert( FindElement( element, element_->list ) );

	return( element_->list );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Wed, May 31, 2000	Created.

	************************************************************************************/

	bool
IsListEmpty(
	ElementList	*list )
{
	assertList( list );

	return( list->first == NULL );
}

/****************************************************************************************
*
*	Grabbers
*
****************************************************************************************/
#pragma mark	-
#pragma mark	(Grabbers)

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.
	wolf		Wed, May 31, 2000	Updated to support the new list field in Element.

	************************************************************************************/

	void
RemoveElement(
	void			*element,
	ElementList		*list )
{
	Element	*element_ = (Element*) element;

	assertElement( element );
	assertList( list );

	if( list->first == element_ )
		list->first = element_->next;
	if( list->last == element_ )
		list->last = element_->prev;
	if( element_->prev )
		element_->prev->next = element_->next;
	if( element_->next )
		element_->next->prev = element_->prev;
	element_->prev = element_->next = NULL;
	element_->list = NULL;

	assert( !FindElement( element, list ) );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
GrabFirstElement(
	void			**element,
	ElementList		*list )
{
	assertPtr( element );
	assertList( list );

	FirstElement( element, list );
	if( *element )
		RemoveElement( *element, list );

	assertIf( *element, !FindElement( *element, list ) );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
GrabLastElement(
	void			**element,
	ElementList		*list )
{
	assertPtr( element );
	assertList( list );

	LastElement( element, list );
	if( *element )
		RemoveElement( *element, list );

	assertIf( *element, !FindElement( *element, list ) );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
GrabNextElement(
	void			*element,
	void			**nextElement,
	ElementList		*list )
{
	assertElement( element );
	assertPtr( nextElement );
	assertList( list );

	NextElement( element, nextElement );
	RemoveElement( element, list );

	assert( !FindElement( element, list ) );
	assertIf( *nextElement, FindElement( *nextElement, list ) );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
GrabPrevElement(
	void			*element,
	void			**prevElement,
	ElementList		*list )
{
	assertElement( element );
	assertPtr( prevElement );
	assertList( list );

	PrevElement( element, prevElement );
	RemoveElement( element, list );

	assert( !FindElement( element, list ) );
	assertIf( *prevElement, FindElement( *prevElement, list ) );
}

/****************************************************************************************
*
*	Offset Putters
*
****************************************************************************************/
#pragma mark	-
#pragma mark	(Offset Putters)

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
PutFirstElementOff(
	void			*element,
	ElementList		*list,
	size_t			offset )
{
	PutFirstElement( AddOffset( element, offset ), list );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
PutLastElementOff(
	void			*element,
	ElementList		*list,
	size_t			offset )
{
	PutLastElement( AddOffset( element, offset ), list );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
PutBeforeElementOff(
	void			*element,
	void			*before,
	ElementList		*list,
	size_t			offset )
{
	PutBeforeElement( AddOffset( element, offset ), AddOffset( before, offset ), list );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
PutAfterElementOff(
	void			*element,
	void			*after,
	ElementList		*list,
	size_t			offset )
{
	PutAfterElement( AddOffset( element, offset ), AddOffset( after, offset ), list );
}



/****************************************************************************************
*
*	Offset Accessors
*
****************************************************************************************/
#pragma mark	-
#pragma mark	(Offset Accessors)

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
FirstElementOff(
	void			**element,
	ElementList		*list,
	size_t			offset )
{
	FirstElement( element, list );
	*element = SubtractOffset( *element, offset );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
LastElementOff(
	void			**element,
	ElementList		*list,
	size_t			offset )
{
	LastElement( element, list );
	*element = SubtractOffset( *element, offset );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
NextElementOff(
	void		*element,
	void		**nextElement,
	size_t		offset )
{
	NextElement( AddOffset( element, offset ), nextElement );
	*nextElement = SubtractOffset( *nextElement, offset );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
PrevElementOff(
	void		*element,
	void		**prevElement,
	size_t		offset )
{
	PrevElement( AddOffset( element, offset ), prevElement );
	*prevElement = SubtractOffset( *prevElement, offset );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Fri, Apr 16, 1999	Created.

	************************************************************************************/

	bool
FindElementOff(
	void			*element,
	ElementList		*list,
	size_t			offset )
{
	return( FindElement( AddOffset( element, offset ), list ) );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Wed, May 31, 2000	Created.

	************************************************************************************/

	ElementList*
GetElementListOff(
	void	*element,
	size_t	offset )
{
	return( GetElementList( AddOffset( element, offset ) ) );
}

/****************************************************************************************
*
*	Offset Grabbers
*
****************************************************************************************/
#pragma mark	-
#pragma mark	(Offset Grabbers)

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
RemoveElementOff(
	void			*element,
	ElementList		*list,
	size_t			offset )
{
	RemoveElement( AddOffset( element, offset ), list );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
GrabFirstElementOff(
	void			**element,
	ElementList		*list,
	size_t			offset )
{
	GrabFirstElement( element, list );
	*element = SubtractOffset( *element, offset );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
GrabLastElementOff(
	void			**element,
	ElementList		*list,
	size_t			offset )
{
	GrabLastElement( element, list );
	*element = SubtractOffset( *element, offset );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
GrabNextElementOff(
	void			*element,
	void			**nextElement,
	ElementList		*list,
	size_t			offset )
{
	GrabNextElement( AddOffset( element, offset ), nextElement, list );
	*nextElement = SubtractOffset( *nextElement, offset );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Tue, Apr 6, 1999	Created.

	************************************************************************************/

	void
GrabPrevElementOff(
	void			*element,
	void			**prevElement,
	ElementList		*list,
	size_t			offset )
{
	GrabPrevElement( AddOffset( element, offset ), prevElement, list );
	*prevElement = SubtractOffset( *prevElement, offset );
}

/****************************************************************************************
*
*	Implementation
*
****************************************************************************************/
#pragma mark	-
#pragma mark	(Private)

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Wed, Apr 14, 1999	Created.

	************************************************************************************/

	void*
AddOffset(
	void	*element,
	size_t	offset )
{
	char	*result = (char*) element;

	assertPtrIfNotNil( element );
	assert( offset >= 0 && offset < 1024 );

	if( element )
		result += offset;

	return( result );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Wed, Apr 14, 1999	Created.

	************************************************************************************/

	void*
SubtractOffset(
	void	*element,
	size_t	offset )
{
	char	*result = (char*) element;

	assertPtrIfNotNil( element );
	assert( offset >= 0 && offset < 1024 );

	if( element )
		result -= offset;

	return( result );
}

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Fri, Apr 16, 1999	Created.

	************************************************************************************/

#if	UseAssertions
	void
assertElement(
	void	*element )
{
	Element	*element_ = (Element*) element;

	assertPtr( element );
	assertPtrIfNotNil( element_->next );
	assertPtrIfNotNil( element_->prev );
}
#endif

/****************************************************************************************
	Commenter	Date				Comment
	---------	-----------------	-----------------------------------------------------
	wolf		Fri, Apr 16, 1999	Created.

	************************************************************************************/

#if	UseAssertions
	void
assertList(
	ElementList	*list )
{
	assertPtr( list );
	assertPtrIfNotNil( list->first );
	assertPtrIfNotNil( list->last );
	//	Either first and last can both be NULL, or neither can be NULL.
	assert( (list->first && list->last) || (!list->first && !list->last) );
}
#endif