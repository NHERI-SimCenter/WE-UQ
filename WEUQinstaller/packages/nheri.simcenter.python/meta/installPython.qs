function Component() {}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows")
    {
        component.addOperation("CreateShortcut", "@TargetDir@/python.exe", "@StartMenuDir@/python.lnk", "workingDirectory=@TargetDir@", "description=SimCenter configured python interpreter");
    }
}
