/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <api/ientorno.h>
#include <main/entorno.h>

GNC::GCS::IEntorno::IEntorno()
{
}

GNC::GCS::IEntorno::~IEntorno()
{
}

GNC::GCS::IEntorno* GNC::GCS::IEntorno::Instance()
{
        return GNC::Entorno::Instance();
}

void GNC::GCS::IEntorno::FreeInstance()
{
        return GNC::Entorno::FreeInstance();
}
