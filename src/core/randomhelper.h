/*
  Copyright (C) 2016 Marcus Soll
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef RANDOMHELPER_H
#define RANDOMHELPER_H

#include <random>
#include <QVector>
#include <QThread>
#include <QTime>
#include <QDate>

namespace RandomHelper {
static bool random_initialised = false;
static std::mt19937 rnd;
static std::uniform_int_distribution<int> place_distribution(0, 7);

inline void initialise()
{
    if(!random_initialised)
    {
        std::random_device random_device;
        QVector<int> seed_vector;
        seed_vector.reserve(4);
        seed_vector << random_device();
        seed_vector << QTime(0,0,0).msecsTo(QTime::currentTime());
        seed_vector << QDate::currentDate().dayOfYear();
        seed_vector << QDate::currentDate().year();
        std::seed_seq seed(seed_vector.begin(), seed_vector.end());
        rnd.seed(seed);
        random_initialised = true;
    }
}

inline int random(int low, int high)
{
    std::uniform_int_distribution<int> distribution(low, high);
    return distribution(rnd);
}

inline int random_place()
{
    return place_distribution(rnd);
}
}

#endif // RANDOMHELPER_H

