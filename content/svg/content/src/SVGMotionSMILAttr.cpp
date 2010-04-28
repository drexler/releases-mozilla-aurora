/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Mozilla SVG project.
 *
 * The Initial Developer of the Original Code is the Mozilla Corporation.
 * Portions created by the Initial Developer are Copyright (C) 2010
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Daniel Holbert <dholbert@mozilla.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

/* representation of a dummy attribute targeted by <animateMotion> element */

#include "SVGMotionSMILAttr.h"
#include "SVGMotionSMILType.h"
#include "nsISMILAnimationElement.h"
#include "nsSMILValue.h"
#include "nsAttrValue.h"
#include "nsGkAtoms.h"
#include "nsDebug.h"
#include "nsCRT.h"
#include "nsSVGLength2.h"
#include "nsSMILParserUtils.h"

namespace mozilla {

nsresult
SVGMotionSMILAttr::ValueFromString(const nsAString& aStr,
                                   const nsISMILAnimationElement* aSrcElement,
                                   nsSMILValue& aValue,
                                   PRBool& aCanCache) const
{
  NS_NOTREACHED("Shouldn't using nsISMILAttr::ValueFromString for parsing "
                "animateMotion's SMIL values.");
  return NS_ERROR_FAILURE;
}

nsSMILValue
SVGMotionSMILAttr::GetBaseValue() const
{
  return nsSMILValue(&SVGMotionSMILType::sSingleton);
}

void
SVGMotionSMILAttr::ClearAnimValue()
{
  mSVGElement->SetAnimateMotionTransform(nsnull);
}

nsresult
SVGMotionSMILAttr::SetAnimValue(const nsSMILValue& aValue)
{
  gfxMatrix matrix = SVGMotionSMILType::CreateMatrix(aValue);
  mSVGElement->SetAnimateMotionTransform(&matrix);
  return NS_OK;
}

const nsIContent*
SVGMotionSMILAttr::GetTargetNode() const
{
  return mSVGElement;
}

} // namespace mozilla
