//
// patch.cpp
//
// MiniSynth Pi - A virtual analogue synthesizer for Raspberry Pi
// Copyright (C) 2017  R. Stange <rsta2@o2online.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "patch.h"
#include "oscillator.h"
#include <assert.h>

struct
{
	const char	*pName;
	TParameterType	 Type;
	unsigned	 nMinimum;
	unsigned	 nMaximum;
	unsigned	 nStep;
	unsigned	 nDefault;
	const char	*pHelp;
}
ParameterList[] =		// must match TSynthParameter
{
	// VCO
	{"LFOVCOWaveform", ParameterWaveform, WaveformSine, WaveformUnknown-1, 1, WaveformSine, "Wave"},
	{"LFOVCOFrequency", ParameterFrequency, 1, 35, 1, 20, "Rate"},

	{"VCOWaveform", ParameterWaveform, WaveformSine, WaveformUnknown-1, 1, WaveformSquare, "Wave"},
	{"VCOModulationVolume", ParameterPercent, 0, 100, 10, 0, "Volume"},

	// VCF
	{"LFOVCFWaveform", ParameterWaveform, WaveformSine, WaveformUnknown-1, 1, WaveformSine, "Wave"},
	{"LFOVCFFrequency", ParameterFrequency, 1, 35, 1, 20, "Rate"},

	{"VCFCutoffFrequency", ParameterPercent, 10, 100, 2, 80, "Cutoff"},
	{"VCFResonance", ParameterPercent, 0, 100, 2, 50, "Resonance"},

	{"EGVCFAttack", ParameterTime, 0, 2000, 50, 0, "Attack"},
	{"EGVCFDecay", ParameterTime, 100, 10000, 100, 4000, "Decay"},
	{"EGVCFSustain", ParameterPercent, 0, 100, 10, 100, "Sustain"},
	{"EGVCFRelease", ParameterTime, 0, 5000, 100, 1000, "Release"},

	{"VCFModulationVolume", ParameterPercent, 0, 90, 5, 0, "Volume"},

	// VCA
	{"LFOVCAWaveform", ParameterWaveform, WaveformSine, WaveformUnknown-1, 1, WaveformSine, "Wave"},
	{"LFOVCAFrequency", ParameterFrequency, 1, 35, 1, 5, "Rate"},

	{"EGVCAAttack", ParameterTime, 0, 2000, 50, 100, "Attack"},
	{"EGVCADecay", ParameterTime, 100, 10000, 100, 4000, "Decay"},
	{"EGVCASustain", ParameterPercent, 0, 100, 10, 100, "Sustain"},
	{"EGVCARelease", ParameterTime, 0, 5000, 100, 100, "Release"},

	{"VCAModulationVolume", ParameterPercent, 0, 90, 10, 0, "Volume"},

	// Synth
	{"SynthVolume", ParameterPercent, 0, 100, 10, 50, "Volume"}
};

CPatch::CPatch (const char *pFileName, CFATFileSystem *pFileSystem)
:	m_Properties (pFileName, pFileSystem)
{
	for (unsigned i = 0; i < SynthParameterUnknown; i++)
	{
		m_pParameter[i] = new CParameter (ParameterList[i].pName,
						  ParameterList[i].Type,
						  ParameterList[i].nMinimum,
						  ParameterList[i].nMaximum,
						  ParameterList[i].nStep,
						  ParameterList[i].nDefault,
						  ParameterList[i].pHelp);
		assert (m_pParameter[i] != 0);
	}
}

CPatch::~CPatch (void)
{
	for (unsigned i = 0; i < SynthParameterUnknown; i++)
	{
		delete m_pParameter[i];
		m_pParameter[i] = 0;
	}
}

boolean CPatch::Load (void)
{
	boolean bResult = m_Properties.Load ();
	if (!bResult)
	{
		m_Properties.RemoveAll ();
	}

	for (unsigned i = 0; i < SynthParameterUnknown; i++)
	{
		assert (m_pParameter[i] != 0);
		m_pParameter[i]->Set (m_Properties.GetNumber (m_pParameter[i]->GetName (),
							      m_pParameter[i]->GetDefault ()));
	}

	return bResult;
}

boolean CPatch::Save (void)
{
	m_Properties.RemoveAll ();

	for (unsigned i = 0; i < SynthParameterUnknown; i++)
	{
		assert (m_pParameter[i] != 0);
		m_Properties.SetNumber (m_pParameter[i]->GetName (), m_pParameter[i]->Get ());
	}

	return m_Properties.Save ();
}

unsigned CPatch::GetParameter (TSynthParameter Parameter) const
{
	assert (m_pParameter[Parameter] != 0);
	return m_pParameter[Parameter]->Get ();
}

void CPatch::SetParameter (TSynthParameter Parameter, unsigned nValue)
{
	assert (m_pParameter[Parameter] != 0);
	m_pParameter[Parameter]->Set (nValue);
}

boolean CPatch::ParameterDown (TSynthParameter Parameter)
{
	assert (m_pParameter[Parameter] != 0);
	return m_pParameter[Parameter]->Down ();
}

boolean CPatch::ParameterUp (TSynthParameter Parameter)
{
	assert (m_pParameter[Parameter] != 0);
	return m_pParameter[Parameter]->Up ();
}

const char *CPatch::GetParameterHelp (TSynthParameter Parameter)
{
	assert (m_pParameter[Parameter] != 0);
	return m_pParameter[Parameter]->GetHelp ();
}

const char *CPatch::GetParameterString (TSynthParameter Parameter)
{
	assert (m_pParameter[Parameter] != 0);
	return m_pParameter[Parameter]->GetString ();
}
