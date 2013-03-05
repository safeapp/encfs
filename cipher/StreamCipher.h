
/*****************************************************************************
 * Author:   Valient Gough <vgough@pobox.com>
 *
 *****************************************************************************
 * Copyright (c) 2013 Valient Gough
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.  
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _STREAMCIPHER_incl_
#define _STREAMCIPHER_incl_

#include "base/Range.h"
#include "base/Registry.h"
#include "base/shared_ptr.h"
#include "base/types.h"

namespace encfs {

class StreamCipher
{
 public:
  static Registry<StreamCipher>& GetRegistry();

  struct Properties {
    Range keySize;
    std::string cipher;
    std::string mode;
    std::string library;
  };

  StreamCipher();
  virtual ~StreamCipher();

  virtual bool setKey(const byte *key, int keyLength) =0;
  virtual bool randomKey(int keyLength) =0;

  virtual bool encrypt(const byte *iv, const byte *in,
                       byte *out, int numBytes) =0;
  virtual bool decrypt(const byte *iv, const byte *in,
                       byte *out, int numBytes) =0;
};

}  // namespace encfs


#endif

