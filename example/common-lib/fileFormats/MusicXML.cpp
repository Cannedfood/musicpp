#include "MusicXML.hpp"

#include <stx/xml.hpp>
#include <stx/file2vector.hpp>
#include <fstream>


namespace example::common {

using namespace stx;
using namespace music;
using namespace music::score;


struct _TrackAttributes {
	unsigned divisions;
};

struct _NoteData {

};

static void _readTrackAttributes(_TrackAttributes* out, xml::node& trackAttributes) {
	if(auto* divs = trackAttributes.child("divisions")) {
		out->divisions = divs->req_child(xml::node::content).content_value<int>();
	}
}

static TrackNote _readNote(_TrackAttributes const& attribs, xml::node& note) {
	TrackNote n;

	if(auto* pitch = note.child("pitch")) {
		xml::node *step   = pitch->child("step");
		xml::node *octave = pitch->child("octave");
		xml::node *alter  = pitch->child("alter");
		if (step)
			step = &step->req_child(xml::node::content);
		if (octave)
			octave = &octave->req_child(xml::node::content);
		if (alter)
			alter = &alter->req_child(xml::node::content);
		n.note.value =
		from_string(step->content_value(), alter ? (Accidental)alter->content_value<int>() : NATURAL);
			n.note.octave = octave->content_value<int>();
	}
	else {
		note.req_child("rest");
		n.note = music::Note::Null();
	}

	{
		unsigned rawDuration = note.req_child("duration").req_child(xml::node::content).content_value<int>();
		n.duration = (Duration::quarter() * rawDuration) / attribs.divisions;
	}

	printf("%s %f\n", to_string(n.note).c_str(), n.duration.in_beats());

	return n;
}

score::Sheet ReadMusicXML(std::istream& istream) {
	std::string source = stx::stream2string(istream);

	stx::arena_allocator alloc;
	stx::xml::node root;
	try {
		root.parse_document(alloc, source.c_str());
	}
	catch(stx::parsing::errors::parsing_error& e) {
		e.diagnose("?", source.c_str());
		throw;
	}

	score::Sheet result;
	xml::node& score_partwise = root.req_child("score-partwise");
	// xml::node& part_list = score_partwise.req_child("part-list");

	for(xml::node& part : score_partwise)
	{
		if(part.type() != xml::node::regular) continue;
		if(part.name() != "part") continue;

		std::string_view partID = part.req_attrib<std::string_view>("id");
		printf("PART: %.*s\n", (int)partID.size(), partID.data());

		Track track;

		_TrackAttributes attribs;

		for(xml::node& measure : part) {
			if(measure.type() != xml::node::regular) continue;
			if(measure.name() != "measure") continue;

			if(xml::node* attr = measure.child("attributes")) {
				_readTrackAttributes(&attribs, *attr);
			}

			for(xml::node& note : measure) {
				if(note.type() != xml::node::regular) continue;
				if(note.name() != "note") continue;
				_readNote(attribs, note);
			}
		}

		result.tracks.emplace_back(std::move(track));
	}

	return result;
}

score::Sheet ReadMusicXML(std::string const& path) {
	std::ifstream file(path);
	return ReadMusicXML(file);
}

} // namespace example::common
