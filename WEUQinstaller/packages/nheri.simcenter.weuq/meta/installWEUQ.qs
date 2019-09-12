function Component() {}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows")
    {
        component.addOperation("CreateShortcut", "@TargetDir@/WE-UQ.exe", "@StartMenuDir@/WE-UQ.lnk", "workingDirectory=@TargetDir@", "description=Start SimCenter WE-UQ App");
    }
}
