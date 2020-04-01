package bgu.spl.mics.application.messages;

import bgu.spl.mics.application.passiveObjects.GadgetAvailableResult;

public class GadgetAvailableEvent extends Event<GadgetAvailableResult> {
    private String gadget;

    public GadgetAvailableEvent(String gadget) {
        this.gadget = gadget;
    }

    public String getGadget() {
        return gadget;
    }

    public void setGadgets(String gadget) {
        this.gadget = gadget;
    }
}
