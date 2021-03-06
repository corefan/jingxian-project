﻿

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Globalization;
using System.Threading;


namespace jingxian.core.runtime.simpl
{
    //using jingxian.core.runtime.castleIntegration;
    using jingxian.core.runtime.simpl.Resources;

    public static class Platform
    {
        private static logging.ILog _logger = logging.LogUtils.GetLogger(typeof(Platform));


        static IApplicationLaunchable BuildApplicationLaunchable(IApplicationContext context, IExtensionRegistry registry)
        {
            IExtension launchableExtension;
            if (!registry.TryGetExtension(context.ApplicationLaunchableId, out launchableExtension))
            {
                string msg = string.Format(CultureInfo.InvariantCulture, "启动 {0} 时发生异常 (没有找到扩展 {1} ).",
                        context.Name, context.ApplicationLaunchableId);
                _logger.Error(msg);
                throw new PlatformConfigurationException(msg);
            }

            return launchableExtension.Build<IApplicationLaunchable>();
        }

        public static int Launch(IApplicationContext context, ICommandLineArguments arguments)
        {
            int exitCode = 1;

            AppDomain.CurrentDomain.UnhandledException += OnUnhandledException;

            Enforce.ArgumentNotNull<IApplicationContext>( context,"context");
            Enforce.ArgumentNotNullOrEmpty(context.ApplicationLaunchableId,"context.ApplicationLaunchableId");

            _logger.Debug("开始启动...");

            try
            {
                using (KernelAdapter containerAdapter = new KernelAdapter())
                {
                    containerAdapter.Connect(typeof(IApplicationContext), context);
                    containerAdapter.Connect(RuntimeConstants.AssemblyLoaderServiceId
                        , typeof(IAssemblyLoaderService)
                        , typeof(AssemblyLoaderService));
                    containerAdapter.Connect(RuntimeConstants.BundleServiceId
                        , typeof(IBundleService)
                        , typeof(BundleService));
                    containerAdapter.Connect(RuntimeConstants.ExtensionRegistryId
                        , typeof(IExtensionRegistry)
                        , typeof(ExtensionRegistry));
                    containerAdapter.Start();

                    IExtensionRegistry registry = containerAdapter.Get<IExtensionRegistry>();
                    IObjectBuilder builder = containerAdapter.Get<IObjectBuilder>();

                    using (IDisposable scope = containerAdapter.Lock())
                    {
                        IExtension[] extensions = registry.GetExtensions(Constants.Points.Services);
                        foreach (IExtension extension in extensions)
                        {

                                containerAdapter.Connect(extension.Id
                                    , getServiceTypes(builder, extension)
                                    , builder.GetType(extension.Implementation));
              
                        }

                        List<object> services = new List<object>();
                        foreach (IExtension extension in extensions)
                        {
                            services.Add(containerAdapter.GetService(extension.Id));
                        }

                        foreach (object instance in services)
                        {
                            MicroKernel.Start(instance, containerAdapter);
                        }
                    }

                    IApplicationLaunchable launchable = BuildApplicationLaunchable(context, registry );
                    exitCode = launchable.Launch(context);

                    containerAdapter.Stop();
                }
            }
            finally
            {
                const string exitMsg = "退出代码 {0}.";
                _logger.InfoFormat(exitMsg, exitCode);
                AppDomain.CurrentDomain.UnhandledException -= OnUnhandledException;
            }
            return exitCode;
        }


        static Type[] getServiceTypes(IObjectBuilder builder, IExtension extension)
        {
            ServiceAttribute[] interfaces = builder.GetType(extension.Implementation)
                                         .GetCustomAttributes(typeof(ServiceAttribute), true) as ServiceAttribute[];
            if (null != interfaces && 0 < interfaces.Length)
            {
                Type[] interfaceTypes = new Type[interfaces.Length];
                for (int i = 0; i < interfaceTypes.Length; ++i)
                {
                    interfaceTypes[i] = interfaces[i].ServiceInterface;
                }
                return interfaceTypes;
            }
            return new Type[] { };
        }
        static void OnUnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            _logger.ErrorFormat("发生未抓住的异常  '{0}' .", e.ExceptionObject.GetType());
            _logger.ErrorFormat("	引发者: '{0}'", sender);
            _logger.ErrorFormat("	程序现在 {0}.", e.IsTerminating ? "中断" : "继续运行");
            _logger.Error(e.ExceptionObject);
        }

    }
}